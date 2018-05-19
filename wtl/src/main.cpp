#include <iostream>
#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>
#include "CGreatDialog.h"

CComModule _Module;

#include "utils.h"

//typedef CWinTraits<WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, WS_EX_APPWINDOW> CMyWindowTraits;

class CMyWindow : public CWindowImpl<CMyWindow, CWindow, CFrameWinTraits>
    , public CPaintBkgrnd<CMyWindow, RGB(0, 0, 255)> {
public:

    typedef CPaintBkgrnd<CMyWindow, RGB(0, 0, 255)> CPaintBkgndBase;
    DECLARE_WND_CLASS(_T("CMyWindowClass"))
    BEGIN_MSG_MAP(CMyWindow)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftBtnDown)
        CHAIN_MSG_MAP(CPaintBkgndBase)
    END_MSG_MAP()

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled) {
        DestroyWindow();
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled) {
        PostQuitMessage(0);
        return 0;
    }  

    LRESULT OnLeftBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled) {
        CGreateDialog dlg;
        //dlg.Create(m_hWnd, CWindow::rcDefault);

        dlg.DoModal();
        return 0;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
    _Module.Init(NULL, hInstance);

    CMyWindow win;

    win.Create(NULL, CWindow::rcDefault, _T("My window"));
    win.ShowWindow(nShowCmd);
    win.UpdateWindow();

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    _Module.Term();

    return msg.wParam;
}