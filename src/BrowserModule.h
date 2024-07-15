#pragma once
#include "Browser_i.h"
#include "Settings.h"
#include "AppWindow.h"
#include "TrayWindow.h"
#include "ITrayEvents.h"
#include "IWebActions.h"
#include "SettingsWindow.h"

class CBrowserModule : 
	public CAtlExeModuleT<CBrowserModule>,
	public ITrayEvents,
	public IWebActions
{
protected:
	// Set different timeout values for debug and release builds
#ifdef _DEBUG
	enum
	{
		IDLE_DURATION = 1000 * 15,				// 15 seconds
		MONITOR_DURATION = 1000 * 30,			// 30 seconds
		MONITOR_INTERVAL = 1000 * 2,			// 2 seconds
		POLLING_INTERVAL = 1000 * 30,			// 1 minute
	};
#else
	enum
	{
		IDLE_DURATION = 1000 * 60 * 5,			// 5 minutes
		MONITOR_DURATION = 1000 * 60 * 10,		// 10 minutes
		MONITOR_INTERVAL = 1000 * 2,			// 2 seconds
		POLLING_INTERVAL = 1000 * 60 * 30		// 30 minutes
	};
#endif
	
	enum
	{
		FADE_IN_DURATION = 1000,				// 1 second
		FADE_IN_INTERVAL = 25					// 50 milliseconds
	};

public:
	DECLARE_LIBID(LIBID_BrowserLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BROWSER, "{288d1ed8-10eb-4a82-865a-2a567dfc195c}")

	static CBrowserModule* GetInstance();

	// CAtlExeModuleT
	HRESULT PreMessageLoop(int nShowCmd) throw();
	HRESULT PostMessageLoop() throw();

	// ITrayEvents
	void OnTrayOpen();
	void OnTrayCheckNews();
	void OnTraySettings();
	void OnTrayHelp();
	void OnTrayDisable();
	void OnTrayAbout();
	void OnTrayExit();

	// IWebActions
	void OnActionClose();
	void OnActionShow();
	void OnActionSettings();
	void OnActionExpand(const CString& _url);
	void OnActionSetParam(const CString& _key, const CString& _value);

protected:
	CBrowserModule(void);
	virtual ~CBrowserModule(void);

private:
	void CheckNews(bool _onDemand = false);

	// Timers
	static VOID CALLBACK PollingTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static VOID CALLBACK FadeInTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

private:
	bool m_bFirstRun;
	bool m_bQuitConfirmationPending;
	DWORD m_dwMonitorStartTime;
	UINT_PTR m_uiPollingTimerId;
	UINT_PTR m_uiMonitorTimerId;
	UINT_PTR m_uiFadeInTimerId;

	CSettings m_settings;
	CTrayWindow m_trayWindow;
	CSettingsWindow m_settingsWindow;
	CComObjectStackEx<CAppWindow> m_browser; // Ref-counted stack object
};
