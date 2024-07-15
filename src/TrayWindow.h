#pragma once
#include "Settings.h"
#include "ITrayEvents.h"
#include "TrayIconImpl.h"

class CTrayWindow :
	public CWindowImpl<CTrayWindow>,
	public CTrayIconImpl<CTrayWindow>
{
public:
	enum 
	{ 
		WND_STYLE = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED,
		WND_EX_STYLE = WS_EX_PALETTEWINDOW
	};

	BEGIN_MSG_MAP(CTrayWindow)
		// Messages
		MESSAGE_HANDLER(WM_CREATE, OnCreate)

		// CTrayIconImpl
		MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)

		// Tray menu commands
		COMMAND_ID_HANDLER(ID_TRAY_OPEN, OnTrayOpen)
		COMMAND_ID_HANDLER(ID_TRAY_CHECKNEWS, OnTrayCheckNews)
		COMMAND_ID_HANDLER(ID_TRAY_SETTINGS, OnTraySettings)
		COMMAND_ID_HANDLER(ID_TRAY_HELP, OnTrayHelp)
		COMMAND_ID_HANDLER(ID_TRAY_DISABLE, OnTrayDisable)
		COMMAND_ID_HANDLER(ID_TRAY_ABOUT, OnTrayAbout)
		COMMAND_ID_HANDLER(ID_TRAY_EXIT, OnTrayExit)

		// Chaining
		CHAIN_MSG_MAP(CTrayIconImpl<CTrayWindow>)
	END_MSG_MAP()

public:
	CTrayWindow(CSettings* _settings);
	virtual ~CTrayWindow(void);

	void Subscribe(ITrayEvents* _listener);

	// CTrayIconImpl
	void PrepareMenu(HMENU hMenu);

protected:
	// Message handlers
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);

	// CTrayIconImpl
	LRESULT OnTrayIcon(UINT, WPARAM, LPARAM, BOOL&);

	// Tray menu command handlers
	LRESULT OnTrayOpen(WORD, WORD, HWND, BOOL&);
	LRESULT OnTrayCheckNews(WORD, WORD, HWND, BOOL&);
	LRESULT OnTraySettings(WORD, WORD, HWND, BOOL&);
	LRESULT OnTrayHelp(WORD, WORD, HWND, BOOL&);
	LRESULT OnTrayDisable(WORD, WORD, HWND, BOOL&);
	LRESULT OnTrayAbout(WORD, WORD, HWND, BOOL&);
	LRESULT OnTrayExit(WORD, WORD, HWND, BOOL&);

private:
	// Types
	typedef CWinTraits<WND_STYLE, WND_EX_STYLE> CAppWinTraits;

private:
	CSettings* m_pSettings;
	CSimpleArray<ITrayEvents*> m_aEventListeners;
};
