/**
 * @author Richard
 * @email richardo2016@gmail.com
 * @create date 2018-04-23 03:25:07
 * @modify date 2018-04-23 03:25:42
 * @desc Webview Object for Mac OSX
*/
#ifdef __APPLE__

#include "ifs/WebView.h"

#ifndef WEBVIEW_H_
#define WEBVIEW_H_

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <objc/runtime.h>

#define DEFAULT_URL                                                              \
    "data:text/"                                                                 \
    "html,%3C%21DOCTYPE%20html%3E%0A%3Chtml%20lang=%22en%22%3E%0A%3Chead%3E%"    \
    "3Cmeta%20charset=%22utf-8%22%3E%3Cmeta%20http-equiv=%22X-UA-Compatible%22%" \
    "20content=%22IE=edge%22%3E%3C%2Fhead%3E%0A%3Cbody%3E%3Cdiv%20id=%22app%22%" \
    "3E%3C%2Fdiv%3E%3Cscript%20type=%22text%2Fjavascript%22%3E%3C%2Fscript%3E%"  \
    "3C%2Fbody%3E%0A%3C%2Fhtml%3E"

#define CSS_INJECT_FUNCTION                                             \
    "(function(e){var "                                                 \
    "t=document.createElement('style'),d=document.head||document."      \
    "getElementsByTagName('head')[0];t.setAttribute('type','text/"      \
    "css'),t.styleSheet?t.styleSheet.cssText=e:t.appendChild(document." \
    "createTextNode(e)),d.appendChild(t)})"

struct webview_priv {
    NSAutoreleasePool* pool;
    NSWindow* window;
    WebView* webview;
    id windowDelegate;
    int should_exit;
};

typedef void (*webview_external_invoke_cb_t)(struct webview* w,
    const char* arg);

struct webview {
    const char* url;
    const char* title;
    int width;
    int height;
    int resizable;
    int debug;
    webview_external_invoke_cb_t external_invoke_cb;
    struct webview_priv priv;
    void* userdata;
};

typedef void (*webview_dispatch_fn)(struct webview* w, void* arg);

struct webview_dispatch_arg {
    webview_dispatch_fn fn;
    struct webview* w;
    void* arg;
};

static void webview_dispatch_cb(void* arg)
{
    struct webview_dispatch_arg* context = (struct webview_dispatch_arg*)arg;
    (context->fn)(context->w, context->arg);
    free(context);
}

static void webview_dispatch(struct webview* w, webview_dispatch_fn fn,
    void* arg)
{
    struct webview_dispatch_arg* context = (struct webview_dispatch_arg*)malloc(
        sizeof(struct webview_dispatch_arg));
    context->w = w;
    context->arg = arg;
    context->fn = fn;
    dispatch_async_f(dispatch_get_main_queue(), context, webview_dispatch_cb);
}

static void webview_terminate(struct webview* w)
{
    w->priv.should_exit = 1;
}
static void webview_exit(struct webview* w) { [NSApp terminate:NSApp]; }
static void webview_print_log(const char* s) { NSLog(@"%s", s); }

namespace fibjs {

class AppWebView : public WebView_base {
    FIBER_FREE();

public:
    AppWebView(exlib::string url, NObject* opt);
    ~AppWebView();

    void open();

    EVENT_SUPPORT();

public:
    // WebView_base
    virtual result_t setHtml(exlib::string html, AsyncEvent* ac);
    virtual result_t print(int32_t mode, AsyncEvent* ac);
    virtual result_t close(AsyncEvent* ac);
    virtual result_t wait(AsyncEvent* ac);
    virtual result_t postMessage(exlib::string msg, AsyncEvent* ac);
    virtual result_t get_visible(bool& retVal);
    virtual result_t set_visible(bool newVal);

public:
    EVENT_FUNC(load);
    EVENT_FUNC(move);
    EVENT_FUNC(resize);
    EVENT_FUNC(closed);
    EVENT_FUNC(message);

private:
    void GoBack();
    void GoForward();
    void Refresh();
    void Navigate(exlib::string szUrl);

private:
    void clear();

    int _initWhenOpen(struct webview* w);
    int _loop(struct webview* w, int blocking);

    // result_t postMessage(exlib::string msg, _variant_t& retVal);
    // result_t WebView::postClose(_variant_t& retVal);

protected:
    exlib::string m_url;
    obj_ptr<NObject> m_opt;

    bool m_visible;
    bool m_maximize;
    bool m_bSilent;

    AsyncEvent* m_ac;
};
} /* namespace fibjs */

#endif /* WEBVIEW_H_ */
#endif /* __APPLE__ */