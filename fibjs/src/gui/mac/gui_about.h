/**
 * @author Richard
 * @email richardo2016@gmail.com
 * @create date 2018-04-23 04:17:45
 * @modify date 2018-04-23 04:17:45
 * @desc [description]
*/

#include "object.h"
#include "ifs/gui.h"
#include "WebView.h"
#include "EventInfo.h"
#include "utf8.h"
#include <exlib/include/thread.h>

#include "pragma.h"

namespace fibjs {

static exlib::LockedList<AsyncEvent> s_uiPool;
static uint32_t s_thread;

struct webview s_activeWin = { 0 };
class non_main_thread : public exlib::OSThread {

public:
    virtual void Run()
    {
        while (true) {
            AsyncEvent* p = s_uiPool.getHead();
            if (p) {
                p->invoke();
                printf("[non_main_thread]I am get \n");
            }

            // MSG msg;
            // GetMessage(&msg, NULL, 0, 0);

            // if (&s_activeWin) {
            // WebView* webView1 = (WebView*)GetWindowLongPtr(s_activeWin, 0);
            // if (webView1 && (webView1->TranslateAccelerator(&msg) == S_OK))
            //     continue;
            // }

            // TranslateMessage(&msg);
            // DispatchMessage(&msg);
        }
    }

public:
    // STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
    // {
    //     if (ppvObj == NULL)
    //         return E_POINTER;

    //     if (pUnkOuter != NULL)
    //         return CLASS_E_NOAGGREGATION;

    //     if (riid != IID_IUnknown && riid != IID_IInternetProtocol && riid != IID_IInternetProtocolInfo)
    //         return E_NOINTERFACE;

    //     return (new FSProtocol())->QueryInterface(riid, ppvObj);
    // }

    // STDMETHODIMP LockServer(BOOL fLock)
    // {
    //     return S_OK;
    // }
};
} /* namespace fibjs */