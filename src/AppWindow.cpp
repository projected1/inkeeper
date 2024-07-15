#include "stdafx.h"
#include "AppWindow.h"

// -----------------------------------------------------------------------------
CAppWindow::CAppWindow(void)
{
	m_bMouseHover = false;
}

// -----------------------------------------------------------------------------
CAppWindow::~CAppWindow(void)
{
}

// -----------------------------------------------------------------------------
LRESULT CAppWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Make this a top-most window
	RECT rc = {0};
	BOOL rv = this->GetWindowRect(&rc);
	ATLENSURE_RETURN_VAL(rv, -1);

	rv = this->SetWindowPos(HWND_TOPMOST, &rc, 0);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Don't grab focus and don't show on the taskbar
	rv = this->ModifyStyleEx(0, WS_EX_NOACTIVATE);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Make this a layered window and set it's initial opacity to 90%
	rv = this->ModifyStyleEx(0, WS_EX_LAYERED);
	ATLENSURE_RETURN_VAL(rv, -1);
	
	SetWindowOpacity(0.9f);

	// Remove window frame
	rv = this->ModifyStyle(WS_CAPTION, 0);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Set window size
	rv = this->ResizeClient(WND_WIDTH, WND_HEIGHT);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Set window position
	//

	// Get unobscured desktop rect
	CRect rcDesktop;
	rv = ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcDesktop, 0);
	ATLENSURE_RETURN_VAL(rv, -1);

	CRect rcWndClient;
	rv = this->GetClientRect(&rcWndClient);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Negate size to be used as a negative offset
	CSize sizeClient = rcWndClient.Size();
	sizeClient.cx = -sizeClient.cx;
	sizeClient.cy = -sizeClient.cy;

	CRect rcWndScreen;
	rv = this->GetWindowRect(rcWndScreen);
	ATLENSURE_RETURN_VAL(rv, -1);

	CPoint pt = rcDesktop.BottomRight();
	pt.Offset(sizeClient);

	// Give the window some margin from the bottom-right corner
	pt.Offset(-30, -30);

	rcWndScreen.MoveToXY(pt);
	rv = this->MoveWindow(rcWndScreen);
	ATLENSURE_RETURN_VAL(rv, -1);

	// Declare "unhandled" for chaining
	bHandled = FALSE;
	return 0;
}

// ----------------------------------------------------------------------------
LRESULT CAppWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bMouseHover)
	{
		// Track mouse-leave event
		TRACKMOUSEEVENT tme = {0};
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hwndBrowser;
		tme.dwHoverTime = HOVER_DEFAULT;
		BOOL rv = _TrackMouseEvent(&tme);
		ATLASSERT(rv);

		// Simulate mouse-enter event
		rv = this->PostMessage(WM_MOUSEENTER, wParam, lParam);
		ATLASSERT(rv);
	}

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
LRESULT CAppWindow::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bMouseHover = true;

	SetWindowOpacity(1);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
LRESULT CAppWindow::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bMouseHover = false;

	SetWindowOpacity(0.9f);

	bHandled = TRUE;
	return 0;
}

// -----------------------------------------------------------------------------
FLOAT CAppWindow::GetWindowOpacity()
{
	BYTE cAlpha = 0;
	BOOL rv = ::GetLayeredWindowAttributes(
		this->m_hWnd,
		NULL,			// Key - transparency
		&cAlpha,		// Alpha - translucency
		NULL);
	ATLASSERT(rv);

	FLOAT fAlpha = (FLOAT)cAlpha / ALPHA_OPAQUE;
	
	return fAlpha;
}

// -----------------------------------------------------------------------------
void CAppWindow::SetWindowOpacity(FLOAT _alpha)
{
	ATLASSERT(_alpha >= 0);
	ATLASSERT(_alpha <= 1);

	BYTE cAlpha = (BYTE)(ALPHA_OPAQUE * _alpha);

	BOOL rv = ::SetLayeredWindowAttributes(
		this->m_hWnd, 
		NULL,			// Key - transparency
		cAlpha,			// Alpha - translucency
		LWA_ALPHA);
	ATLASSERT(rv);
}
