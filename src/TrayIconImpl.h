#pragma once
#include <shellapi.h>

template <class T>
class CTrayIconImpl
{
protected:
	enum { WM_TRAYICON = (WM_USER + __COUNTER__) };

private:
	BOOL m_bInstalled;
	UINT m_uiDefault;
	NOTIFYICONDATA m_nid;

public:	
	BEGIN_MSG_MAP(CTrayIcon)
		MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
	END_MSG_MAP()

	// ------------------------------------------------------------------------
	CTrayIconImpl() 
	{
		m_bInstalled = false;
		m_uiDefault = 0;

		::ZeroMemory(&m_nid, sizeof(NOTIFYICONDATA));
		m_nid.cbSize = sizeof(NOTIFYICONDATA);
	}
	
	// ------------------------------------------------------------------------
	virtual ~CTrayIconImpl()
	{
		RemoveIcon();
	}

	// ------------------------------------------------------------------------
	BOOL InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
	{
		T* pT = static_cast<T*>(this);

		m_nid.hWnd = pT->m_hWnd;
		m_nid.uID = nID;
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_nid.uCallbackMessage = WM_TRAYICON;
		_tcscpy_s(m_nid.szTip, _countof(m_nid.szTip), lpszToolTip);

		m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid);
		return m_bInstalled;
	}

	// ------------------------------------------------------------------------
	BOOL RemoveIcon()
	{
		if (!m_bInstalled)
			return FALSE;

		m_nid.uFlags = 0;
		return Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}

	// ------------------------------------------------------------------------
	BOOL SetTooltipText(LPCTSTR pszTooltipText)
	{
		if (pszTooltipText == NULL)
			return FALSE;

		m_nid.uFlags = NIF_TIP;
		_tcscpy(m_nid.szTip, pszTooltipText);

		return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}

	// ------------------------------------------------------------------------
	inline void SetDefaultItem(UINT nID) 
	{ 
		m_uiDefault = nID; 
	}

	// ------------------------------------------------------------------------
	LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam != m_nid.uID)
			return 0;

		T* pT = static_cast<T*>(this);

		if (LOWORD(lParam) == WM_RBUTTONUP)
		{
			CMenu menu;
			if (!menu.LoadMenu(m_nid.uID))
				return 0;

			CMenuHandle popupMenu(menu.GetSubMenu(0));
			pT->PrepareMenu(popupMenu);

			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow(pT->m_hWnd);

			if (m_uiDefault == 0)
				popupMenu.SetMenuDefaultItem(0, TRUE);
			else
				popupMenu.SetMenuDefaultItem(m_uiDefault);

			popupMenu.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pT->m_hWnd);

			// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
			pT->PostMessage(WM_NULL);

			menu.DestroyMenu();
		}
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
		{
			SetForegroundWindow(pT->m_hWnd);

			CMenu menu;
			if (!menu.LoadMenu(m_nid.uID))
				return 0;

			CMenuHandle popupMenu(menu.GetSubMenu(0));			
			if (m_uiDefault)
				pT->SendMessage(WM_COMMAND, m_uiDefault, 0);
			else
			{
				UINT nItem = popupMenu.GetMenuItemID(0);
				pT->SendMessage(WM_COMMAND, nItem, 0);
			}

			menu.DestroyMenu();
		}

		bHandled = TRUE;
		return 0;
	}

	// ------------------------------------------------------------------------
	// Allows menu items to be enabled, checked, modified, etc.
	virtual void PrepareMenu(HMENU hMenu)
	{
		// TODO: Override as needed
	}
};
