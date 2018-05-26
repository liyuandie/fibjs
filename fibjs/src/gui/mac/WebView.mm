#ifdef __APPLE__

#include "object.h"
#include "ifs/gui.h"
#include "ifs/fs.h"
#include "ifs/registry.h"
#include "ifs/os.h"
#include "path.h"
#include "WebView.h"
#include "EventInfo.h"
#include "utf8.h"
#include <exlib/include/thread.h>

static const char* webview_check_url(const char* url)
{
    if (url == NULL || strlen(url) == 0) {
        return DEFAULT_URL;
    }
    return url;
}

static void webview_window_will_close(id self, SEL cmd, id notification)
{
    struct webview* w = (struct webview*)objc_getAssociatedObject(self, "webview");
    webview_terminate(w);
}

static BOOL webview_is_selector_excluded_from_web_script(id self, SEL cmd,
    SEL selector)
{
    return selector != @selector(invoke:);
}

static NSString* webview_webscript_name_for_selector(id self, SEL cmd,
    SEL selector)
{
    return selector == @selector(invoke:) ? @"invoke" : nil;
}

static void webview_did_clear_window_object(id self, SEL cmd, id webview,
    id script, id frame)
{
    [script setValue:self forKey:@"external"];
}

static void webview_external_invoke(id self, SEL cmd, id arg)
{
    struct webview* w = (struct webview*)objc_getAssociatedObject(self, "webview");
    if (w == NULL || w->external_invoke_cb == NULL) {
        return;
    }
    if ([arg isKindOfClass:[NSString class]] == NO) {
        return;
    }
    w->external_invoke_cb(w, [(NSString*)(arg)UTF8String]);
}

namespace fibjs {
AppWebView::AppWebView(exlib::string url, NObject* opt)
{
    m_url = url;
    m_opt = opt;

    m_ac = NULL;

    // _onmessage = NULL;
    // _onclose = NULL;

    m_visible = true;

    // RegMainClass();
}

AppWebView::~AppWebView()
{
    clear();
}

void AppWebView::open()
{
    // this method is running in main-thread

    // webview_initWhenOpen(&w);
    printf("\nAppWebView::open");
    // webview("Minimal webview example", "http://fibjs.org", 800, 600, 1);
    const char* title = "Minimal webview example";
    const char* url = "http://fibjs.org";
    int width = 0;
    int height = 0;
    int resizable = 0;

    struct webview webview = { 0 };
    webview.title = title;
    webview.url = url;
    webview.width = width;
    webview.height = height;
    webview.resizable = resizable;
    printf("\nAppWebView::here");

    int r = _initWhenOpen(&webview);
    printf("\nAppWebView::webview_initWhenOpen -- %d", r);
    if (r != 0) {
        return ;
        // return r;
    }
    // while (_loop(&webview, 1) == 0) {
    //     printf("\nAppWebView::webview_loop");
    // }
    // _exit(&webview);
    // printf("\nAppWebView::webview_exit");
}

void AppWebView::clear()
{

    if (m_ac) {
        m_ac->post(0);
        m_ac = NULL;
    }
}

int AppWebView::_initWhenOpen(webview* w)
{
    printf("\nAppWebView::_initWhenOpen");
    // w->priv.pool = [[NSAutoreleasePool alloc] init];
    
    printf("\nAppWebView::_initWhenOpen 3");

    [NSApplication sharedApplication];
    printf("\nAppWebView::_initWhenOpen 4");

    Class webViewDelegateClass = objc_allocateClassPair([NSObject class], "WebViewDelegate", 0);
    
    class_addMethod(webViewDelegateClass, sel_registerName("windowWillClose:"),
        (IMP)webview_window_will_close, "v@:@");
    class_addMethod(object_getClass(webViewDelegateClass),
        sel_registerName("isSelectorExcludedFromWebScript:"),
        (IMP)webview_is_selector_excluded_from_web_script, "c@::");
    class_addMethod(object_getClass(webViewDelegateClass),
        sel_registerName("webScriptNameForSelector:"),
        (IMP)webview_webscript_name_for_selector, "c@::");
    class_addMethod(webViewDelegateClass,
        sel_registerName("webView:didClearWindowObject:forFrame:"),
        (IMP)webview_did_clear_window_object, "v@:@@@");
    class_addMethod(webViewDelegateClass, sel_registerName("invoke:"),
        (IMP)webview_external_invoke, "v@:@");
    objc_registerClassPair(webViewDelegateClass);

    w->priv.windowDelegate = [[webViewDelegateClass alloc] init];
    objc_setAssociatedObject(w->priv.windowDelegate, "webview", (id)(w),
        OBJC_ASSOCIATION_ASSIGN);
    printf("\nAppWebView::_initWhenOpen 5");

    NSString* nsTitle = [NSString stringWithUTF8String:w->title];
    NSRect r = NSMakeRect(0, 0, w->width, w->height);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
    if (w->resizable) {
        style = style | NSWindowStyleMaskResizable;
    }
    
    printf("\nAppWebView::_initWhenOpen 6");
    
    w->priv.window = [[NSWindow alloc] initWithContentRect:r
                                                 styleMask:style
                                                   backing:NSBackingStoreBuffered
                                                     defer:NO];
    printf("\nAppWebView::_initWhenOpen 7");
                                                     
    // [w->priv.window autorelease];
    // [w->priv.window setTitle:nsTitle];
    // [w->priv.window setDelegate:w->priv.windowDelegate];
    // [w->priv.window center];

    // [[NSUserDefaults standardUserDefaults] setBool:!!w->debug
    //                                         forKey:@"WebKitDeveloperExtras"];
    // [[NSUserDefaults standardUserDefaults] synchronize];
    // w->priv.webview =
    //     [[WebView alloc] initWithFrame:r
    //                          frameName:@"WebView"
    //                          groupName:nil];
    // NSURL* nsURL = [NSURL
    //     URLWithString:[NSString stringWithUTF8String:webview_check_url(w->url)]];
    // [[w->priv.webview mainFrame] loadRequest:[NSURLRequest requestWithURL:nsURL]];

    // [w->priv.webview setAutoresizesSubviews:YES];
    // [w->priv.webview
    //     setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    // w->priv.webview.frameLoadDelegate = w->priv.windowDelegate;
    // [[w->priv.window contentView] addSubview:w->priv.webview];
    // [w->priv.window orderFrontRegardless];

    // [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    // [NSApp run];
    printf("\nrun");
    // [NSApp finishLaunching];
    printf("\nfinishLaunching");
    // [NSApp activateIgnoringOtherApps:YES];

    // NSMenu* menubar = [[[NSMenu alloc] initWithTitle:@""] autorelease];

    // NSString* appName = [[NSProcessInfo processInfo] processName];
    // NSMenuItem* appMenuItem =
    //     [[[NSMenuItem alloc] initWithTitle:appName action:NULL keyEquivalent:@""]
    //         autorelease];
    // NSMenu* appMenu = [[[NSMenu alloc] initWithTitle:appName] autorelease];
    // [appMenuItem setSubmenu:appMenu];
    // [menubar addItem:appMenuItem];

    // NSString* title = [@"Hide " stringByAppendingString:appName];
    // NSMenuItem* item = [[[NSMenuItem alloc] initWithTitle:title
    //                                                action:@selector(hide:)
    //                                         keyEquivalent:@"h"] autorelease];
    // [appMenu addItem:item];
    // item = [[[NSMenuItem alloc] initWithTitle:@"Hide Others"
    //                                    action:@selector(hideOtherApplications:)
    //                             keyEquivalent:@"h"] autorelease];
    // [item setKeyEquivalentModifierMask:(NSEventModifierFlagOption | NSEventModifierFlagCommand)];
    // [appMenu addItem:item];
    // item = [[[NSMenuItem alloc] initWithTitle:@"Show All"
    //                                    action:@selector(unhideAllApplications:)
    //                             keyEquivalent:@""] autorelease];
    // [appMenu addItem:item];
    // [appMenu addItem:[NSMenuItem separatorItem]];

    // title = [@"Quit " stringByAppendingString:appName];
    // item = [[[NSMenuItem alloc] initWithTitle:title
    //                                    action:@selector(terminate:)
    //                             keyEquivalent:@"q"] autorelease];
    // [appMenu addItem:item];

    // [NSApp setMainMenu:menubar];

    w->priv.should_exit = 0;
    return 0;
}

int AppWebView::_loop(struct webview* w, int blocking)
{
    NSDate* until = (blocking ? [NSDate distantFuture] : [NSDate distantPast]);
    NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                        untilDate:until
                                           inMode:NSDefaultRunLoopMode
                                          dequeue:YES];
    if (event) {
        [NSApp sendEvent:event];
    }
    return w->priv.should_exit;
}

result_t AppWebView::setHtml(exlib::string html, AsyncEvent* ac)
{
    return 0;
};

result_t AppWebView::print(int32_t mode, AsyncEvent* ac)
{
    if (ac->isSync())
        return CHECK_ERROR(CALL_E_GUICALL);

    return 0;
};

result_t AppWebView::close(AsyncEvent* ac)
{
    return 0;
};

result_t AppWebView::wait(AsyncEvent* ac)
{
    if (ac->isSync())
        return CHECK_ERROR(CALL_E_GUICALL);

    m_ac = ac;
    return CALL_E_PENDDING;
};

result_t AppWebView::postMessage(exlib::string msg, AsyncEvent* ac)
{
    if (ac->isSync())
        return CHECK_ERROR(CALL_E_GUICALL);

    // PostMessage(hWndParent, WM_CLOSE, 0, 0);
    return 0;
};

result_t AppWebView::get_visible(bool& retVal)
{
    return 0;
};

result_t AppWebView::set_visible(bool newVal)
{
    return 0;
};
} /* namespace fibjs */
#endif /* __APPLE__ */