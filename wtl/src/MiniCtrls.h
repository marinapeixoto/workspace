/**
 * tkorays@hotmail.com
 */
#pragma once
#include <atlapp.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlgdi.h>
#include <atlddx.h>
#include <atlmisc.h>
#include <string>

#define UM_CTRL_MSG_CLICK WM_USER + 1
#define UM_CTRL_MSG_PAINT WM_USER + 2
/* 一个背景绘制的类 */
template <class T, COLORREF t_crBrushColor>
class CPaintBg : public CMessageMap {
public:
    CPaintBg() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
    ~CPaintBg() { DeleteObject(m_hbrBkgnd); }

    BEGIN_MSG_MAP(CPaintBg)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled) {
        T* pT = static_cast<T*>(this);
        HDC dc = (HDC)wParam;
        RECT rcClient;

        pT->GetClientRect(&rcClient);
        FillRect(dc, &rcClient, m_hbrBkgnd);
        return 1;
    }
private:
    HBRUSH m_hbrBkgnd;
};
#define MC_CTRL_STYLE_BORDER 0x0001
#define MC_CTRL_STYLE_BKGND  0x0002


template <class T, UINT style, COLORREF crBg = RGB(255,255,255), COLORREF crBorder = RGB(255, 255, 255), UINT IdOnClick = 0>
class ICCommonCtrl : public CWindowImpl<T>
{
public:
    ICCommonCtrl() { 
        m_hbrBkgnd = CreateSolidBrush(crBg); 
        m_hpBorder = ::CreatePen(PS_SOLID, 1, crBorder);
    }
    ~ICCommonCtrl() { 
        DeleteObject(m_hbrBkgnd); 
        DeleteObject(m_hpBorder); 
    }

    DECLARE_WND_CLASS(_T("ICCommonCtrl"))
    BEGIN_MSG_MAP(ICCommonCtrl)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()

    LRESULT OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
        SendMessage(GetParent(), IdOnClick, (WPARAM)0, (LPARAM)0);
        return 0;
    }

    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled) {
        T* pT = static_cast<T*>(this);
        HDC dc = (HDC)wParam;

        if(style & MC_CTRL_STYLE_BKGND) {
            RECT rcClient;
            pT->GetClientRect(&rcClient);
            SelectObject(dc, m_hbrBkgnd);
            FillRect(dc, &rcClient, m_hbrBkgnd);
        }

        if(style & MC_CTRL_STYLE_BORDER) {
            RECT rc;
            ::GetWindowRect(pT->m_hWnd, &rc);
            SelectObject(dc, m_hpBorder);
            Rectangle(dc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

           
        }

        return 1;
    }
private:
    HBRUSH m_hbrBkgnd;
    HPEN m_hpBorder;
};

/* 一个按钮的模板类 */
template <COLORREF crBkColor, UINT idOnClick>
class CRectButton : public CWindowImpl<CRectButton>,
    public CPaintBg<CRectButton, crBkColor>
{
public:
    typedef CPaintBg<CRectButton, crBkColor> _tCPaintBgForThisClass;

    DECLARE_WND_CLASS(_T("CRectButton"))
    BEGIN_MSG_MAP(CRectButton)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
        CHAIN_MSG_MAP(_tCPaintBgForThisClass)
    END_MSG_MAP()

    LRESULT OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
        SendMessage(GetParent(), idOnClick, (WPARAM)0, (LPARAM)0);
        return 0;
    }
};
class ABtn : public ICCommonCtrl<ABtn, 3, RGB(255,255,255), RGB(0,0,0), UM_CTRL_MSG_CLICK> {	
public:
    ABtn(){}
    ~ABtn(){}
} ;

/* 一个纯白背景的弹出对话框 */
template <UINT width, UINT height>
class CPopupDialog : public CIndirectDialogImpl<CPopupDialog>,
    public CPaintBg<CPopupDialog, RGB(255, 255, 255)>
{
public:
    CPopupDialog(){}
    ~CPopupDialog(){}
#define TOP_BTN_WIDTH 30
#define TOP_BTN_HEIGHT 30


    BEGIN_DIALOG(0, 0, width, height)
		DIALOG_CAPTION(_T(""))
		DIALOG_STYLE(WS_POPUP)
		DIALOG_FONT(10, _T("System"))
	END_DIALOG()
    
    BEGIN_CONTROLS_MAP()
		//CONTROL_DEFPUSHBUTTON(_T("OK"), IDOK, 130, 81, 50, 14, 0, 0)
		//CONTROL_CTEXT(_T("Dialog"), IDC_STATIC, 0, 0, 78, 15, 0, 0)
        //CONTROL_CTEXT(_T("x"), IDC_MY_STATIC, 180, 0, 20, 20, 0, WS_EX_TRANSPARENT)
		//CONTROL_ICON(MAKEINTRESOURCE(IDR_MAINFRAME), IDC_STATIC, 55, 26, 18, 20, 0, 0)
		//CONTROL_GROUPBOX(_T(""), IDC_STATIC, 7, 7, 115, 88, 0, 0)
	END_CONTROLS_MAP()

    typedef CPaintBg<CPopupDialog, RGB(255, 255, 255)> _tCPaintBgForThisClass;
	BEGIN_MSG_MAP(CAboutDlgIndirect)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
        MESSAGE_HANDLER(UM_CTRL_MSG_CLICK, OnChildMsg)

        CHAIN_MSG_MAP(_tCPaintBgForThisClass)
	END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
        RECT rc, rcClient;
        ::GetClientRect(m_hWnd, &rcClient);
        ::GetClientRect(m_hWnd, &rc);

        rc.left = rc.right - TOP_BTN_WIDTH;
        rc.bottom = rc.top + TOP_BTN_HEIGHT;
        m_btnClose.Create(m_hWnd, rc, _T("X"), WS_CHILD);
        m_btnClose.ShowWindow(SW_SHOW);

        RECT rc1 = {rcClient.right - 20 - 70, rcClient.bottom - 30 - 20, 
            rcClient.right - 20, rcClient.bottom - 20};
        m_btnOk.Create(m_hWnd, rc1, _T("OK"), WS_CHILD);
        //m_btnOk.SetTitle("OK");
        m_btnOk.ShowWindow(SW_SHOW);
        
#if 1
        ::GetClientRect(m_hWnd, &rc);
        rc.right = rc.left + 80;
        rc.bottom = TOP_BTN_HEIGHT;
        m_sTitle.Create(m_hWnd, rc, "abc", WS_CHILD | SS_CENTERIMAGE);
        m_sTitle.ShowWindow(SW_SHOW);
#endif

		CenterWindow(GetParent());

		return TRUE;
	}

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        
        return TRUE;
    }

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        EndDialog(IDCANCEL);
        return TRUE;
    }

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		RECT rc;
        POINT pt;

        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);

        ::ScreenToClient(m_hWnd, &pt);
        ::GetClientRect(m_hWnd, &rc);

        if(pt.x > rc.left && pt.x < rc.right && pt.y > rc.top && pt.y < rc.bottom) {
            return HTCAPTION;
        }
        return 0;
	}

    LRESULT OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
        EndDialog(IDCANCEL);
        return 0;
    }    


    LRESULT OnChildMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
        if(uMsg == UM_CTRL_MSG_CLICK) {
            EndDialog(IDCANCEL);
        } else if (uMsg == UM_CTRL_MSG_PAINT) {
            
        }
        return 0;
    }
private:
    //CRectButton<RGB(255, 255, 255)> m_btnClose;
    
private:
    template <UINT idOnClick>
    class CBorderButton : public CRectButton<RGB(255, 255, 255), idOnClick>
    {
    public:
        typedef CRectButton<RGB(255, 255, 255), idOnClick> _tRectBtnForThisClass;
        DECLARE_WND_CLASS(_T("CBorderButton"))
        BEGIN_MSG_MAP(CRectButton)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
            CHAIN_MSG_MAP(_tRectBtnForThisClass)
        END_MSG_MAP()

        LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
            HDC dc = ::GetDC(m_hWnd);
            HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            SelectObject(dc, pen);
            
            RECT rc;
            ::GetWindowRect(m_hWnd, &rc);
            Rectangle(dc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);
            DeleteObject(pen);
            ReleaseDC(dc);

            return 0;
        }

        void SetTitle(char* title) {
            
        }
    };

    class CCloseButton : public ICCommonCtrl<CCloseButton, 1, RGB(255,255,255), RGB(0,0,0), UM_CTRL_MSG_CLICK>
    {
    public:
        CCloseButton(){}
        ~CCloseButton(){} 
        typedef ICCommonCtrl<CCloseButton, 1, RGB(255,255,255), RGB(0,0,0), UM_CTRL_MSG_CLICK> _tRectBtnForThisClass;
        DECLARE_WND_CLASS(_T("CCloseButton"))
        BEGIN_MSG_MAP(CCloseButton)
            MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
            CHAIN_MSG_MAP(_tRectBtnForThisClass)
        END_MSG_MAP()

        LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
            ((_tRectBtnForThisClass*)this)->OnEraseBkgnd(uMsg, wParam, lParam, bHandled);

            HDC dc = ::GetDC(m_hWnd);
            HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            SelectObject(dc, pen);

            LineTo(dc, TOP_BTN_WIDTH, TOP_BTN_HEIGHT);
            MoveToEx(dc, 0, TOP_BTN_WIDTH, NULL);
            LineTo(dc, TOP_BTN_WIDTH, 0);
            DeleteObject(pen);

            //bHandled = FALSE;
            return 0;
        }
    };

    CCloseButton m_btnClose;
    //CBorderButton<UM_CTRL_MSG_CLICK> m_btnOk;
    //ICCommonCtrl<ICCommonCtrl, 3, RGB(255,255,255), RGB(0,0,0), UM_CTRL_MSG_CLICK> m_btnOk;
    //ICCommonCtrl<ICCommonCtrl<ICCommonCtrl, 3, RGB(255,255,255), RGB(0,0,0), UM_CTRL_MSG_CLICK>,3> m_btnOk;
    ABtn m_btnOk;	
    CStatic m_sTitle;
};
