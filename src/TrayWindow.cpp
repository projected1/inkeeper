#include "stdafx.h"
#include "Utils.h"
#include "TrayWindow.h"

// -----------------------------------------------------------------------------
CTrayWindow::CTrayWindow(CSettings* _settings)
{
	ATLASSERT(_settings);
	m_pSettings = _settings;
}

// -----------------------------------------------------------------------------
CTrayWindow::~CTrayWindow(void)
{
}

// -----------------------------------------------------------------------------
void CTrayWindow::Subscribe(ITrayEvents* _listener)
{
	ATLASSERT(_listener);
	m_aEventListeners.Add(_listener);
}

// -----------------------------------------------------------------------------
LRESULT CTrayWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Set window styles
	this->SetWindowLong(GWL_STYLE, CAppWinTraits::GetWndStyle(0));
	this->SetWindowLong(GWL_EXSTYLE, CAppWinTraits::GetWndExStyle(0));

	// Install tray icon
	HICON hIcon = (HICON)::LoadImage(
		_AtlBaseModule.m_hInst, 
		MAKEINTRESOURCE(IDI_APP_ICON), 
		IMAGE_ICON, 
		::GetSystemMetrics(SM_CXSMICON), 
		::GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR);
	ATLENSURE_RETURN_VAL(hIcon, -1);

	CString sAppName(Utils::LoadStringFromResource(IDS_APP_NAME));	
	BOOL rv = this->InstallIcon(sAppName, hIcon, IDR_MENU_TRAY);
	ATLENSURE_RETURN_VAL(rv, -1);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// CTrayIconImpl
LRESULT CTrayWindow::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (LOWORD(lParam))
	{
	case WM_RBUTTONUP:
		{
			// Delegate to parent (CTrayIconImpl) who shows the default tray menu
			CTrayIconImpl<CTrayWindow>* pParent = 
				static_cast<CTrayIconImpl<CTrayWindow>*>(this);
			return pParent->OnTrayIcon(uMsg, wParam, lParam, bHandled);
		}
		break;

	case WM_LBUTTONUP:
		{
			this->PostMessage(WM_COMMAND, ID_TRAY_OPEN);
		}
		break;
	}

	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayOpen();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayCheckNews(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayCheckNews();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTraySettings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTraySettings();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayHelp();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayDisable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayDisable();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayAbout();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CTrayWindow::OnTrayExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for (int i = 0; i < m_aEventListeners.GetSize(); i++)
		m_aEventListeners[i]->OnTrayExit();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// CTrayIconImpl
void CTrayWindow::PrepareMenu(HMENU hMenu)
{
	if (m_pSettings->disabled)
	{
		CString sDisabled;
		sDisabled.LoadString(IDS_ENABLE);

		CMenuHandle menu(hMenu);
		BOOL rv = menu.ModifyMenu(
			ID_TRAY_DISABLE, 
			MF_BYCOMMAND | MF_STRING, 
			ID_TRAY_DISABLE, 
			sDisabled);
		ATLASSERT(rv);
	}
}
