#pragma once

template <class T, COLORREF t_crBrushColor>
class CPaintBkgrnd : public CMessageMap {
public:
    CPaintBkgrnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
    ~CPaintBkgrnd() { DeleteObject(m_hbrBkgnd); }

    BEGIN_MSG_MAP(CPaintBkgrnd)
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