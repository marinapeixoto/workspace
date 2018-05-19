#ifndef __CGREATE_DIALOG_H__
#define __CGREATE_DIALOG_H__

#include <atlapp.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include "utils.h"

#define IDC_MY_STATIC 1025

class CMyBtn : public CWindowImpl<CMyBtn>,
    public CPaintBkgrnd<CMyBtn, RGB(255, 255, 255)>
{
public:
    typedef CPaintBkgrnd<CMyBtn, RGB(255, 255, 255)> CMyPaintBkgrnd;
    DECLARE_WND_CLASS(_T("CMyBtn"))
    BEGIN_MSG_MAP(CMyBtn)
        //MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLbtnDown)
        CHAIN_MSG_MAP(CMyPaintBkgrnd)
    END_MSG_MAP()
};

class CBtnCtrl : public CWindowImpl<CBtnCtrl>,
    public CPaintBkgrnd<CBtnCtrl, RGB(0, 0, 0)>
{
public:
    typedef CPaintBkgrnd<CBtnCtrl, RGB(0, 0, 0)> CMyPaintBkgrnd;
    DECLARE_WND_CLASS(_T("CBtnCtrl"))
    BEGIN_MSG_MAP(CBtnCtrl)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLbtnDown)
        CHAIN_MSG_MAP(CMyPaintBkgrnd)
    END_MSG_MAP()



    LRESULT OnLbtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        SendMessage(GetParent(), WM_DESTROY, NULL, NULL);
        return 0;
    }
};

class CGreateDialog : public CIndirectDialogImpl<CGreateDialog>,
    public CPaintBkgrnd<CGreateDialog, RGB(255, 255, 255)>
{
public:
    BEGIN_DIALOG(0, 0, 200, 120)
		DIALOG_CAPTION(_T(""))
		DIALOG_STYLE(WS_POPUP)
		DIALOG_FONT(8, _T("System"))
	END_DIALOG()

	BEGIN_CONTROLS_MAP()
		CONTROL_DEFPUSHBUTTON(_T("OK"), IDOK, 130, 81, 50, 14, 0, 0)
		CONTROL_CTEXT(_T("Dialog"), IDC_MY_STATIC, 0, 0, 78, 20, 0, 0)
        //CONTROL_CTEXT(_T("x"), IDC_MY_STATIC, 180, 0, 20, 20, 0, WS_EX_TRANSPARENT)
		//CONTROL_ICON(MAKEINTRESOURCE(IDR_MAINFRAME), IDC_STATIC, 55, 26, 18, 20, 0, 0)
		//CONTROL_GROUPBOX(_T(""), IDC_STATIC, 7, 7, 115, 88, 0, 0)
	END_CONTROLS_MAP()


    typedef CPaintBkgrnd<CGreateDialog, RGB(255, 255, 255)> CPaintBkgndBase;
	BEGIN_MSG_MAP(CAboutDlgIndirect)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
        CHAIN_MSG_MAP(CPaintBkgndBase)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
        RECT rc;

        ::GetClientRect(m_hWnd, &rc);
        rc.left = rc.right - 40;
        rc.bottom = rc.top + 40;
        
        m_btnClose.Create(m_hWnd, rc, _T("X"), WS_CHILD, 0, IDC_MY_STATIC);
        m_btnClose.ShowWindow(SW_SHOW);

        m_btn1.Create(m_hWnd, rc, _T("sss"), WS_CHILD);
        m_btn1.ShowWindow(SW_SHOW);


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
        
        return 0;
    }
private:
    CStatic m_btnClose;
    CBtnCtrl m_btn1;
};

#endif /* __CGREATE_DIALOG_H__ */
