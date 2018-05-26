#ifdef __APPLE__
// void NSApplicationMain(int argc, char* argv[])
// {
//     [[NSApplication sharedApplication] run];
// }

// #include "stdlib.h"
#include "gui_about.h"

namespace fibjs {

DECLARE_MODULE(gui);

void putGuiPool(AsyncEvent* ac)
{
    s_uiPool.putTail(ac);
    printf("[putGuiPool]I am get \n");
    
    dispatch_async(dispatch_get_main_queue(), ^{
        printf("[putGuiPool lambda]I am ge 3t \n");
    });
}

void run_gui()
{
    printf("running in C++, I am here 12321312");
    
    non_main_thread* _nonMainTh = new non_main_thread();
    _nonMainTh->bindCurrent();
    _nonMainTh->Run();
}

static result_t async_open(obj_ptr<AppWebView> w)
{
    w->open();
    return 0;
}

result_t gui_base::open(exlib::string url, v8::Local<v8::Object> opt, obj_ptr<WebView_base>& retVal)
{
    obj_ptr<NObject> o = new NObject();
    o->add(opt);

    obj_ptr<AppWebView> w = new AppWebView(url, o);
    w->wrap();

    asyncCall(async_open, w, CALL_E_GUICALL);

    retVal = w;
    return 0;
}

result_t gui_base::setVersion(int32_t ver)
{
    return 0;
}
}

#endif
