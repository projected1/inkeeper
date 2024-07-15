#include "stdafx.h"
#include "Utils.h"
#include "BrowserModule.h"


// -----------------------------------------------------------------------------
/*static*/ CBrowserModule* CBrowserModule::GetInstance()
{
	static CBrowserModule instance;
	return &instance;
}

// -----------------------------------------------------------------------------
CBrowserModule::CBrowserModule(void)
:m_settingsWindow(&m_settings), m_trayWindow(&m_settings)
{
	// Seed random numbers
	srand((unsigned)time(NULL));

	m_bFirstRun = false;
	m_bQuitConfirmationPending = false;
	m_dwMonitorStartTime = 0;
	m_uiPollingTimerId = 0;
	m_uiMonitorTimerId = 0;
	m_uiFadeInTimerId = 0;
}

// -----------------------------------------------------------------------------
CBrowserModule::~CBrowserModule(void)
{
	while (m_browser.m_dwRef)
		m_browser.Release();
}

// -----------------------------------------------------------------------------
// CAtlExeModuleT
HRESULT CBrowserModule::PreMessageLoop(int nShowCmd) throw()
{
	UNREFERENCED_PARAMETER(nShowCmd);

	// Create a window for tray icon management and notifications
	HWND hwnd = m_trayWindow.Create(::GetDesktopWindow());
	ATLENSURE_RETURN(hwnd);

	// Subscribe for tray menu events
	m_trayWindow.Subscribe(this);

	// Create a hidden settings window
	hwnd = m_settingsWindow.Create(::GetDesktopWindow());
	ATLENSURE_RETURN(hwnd);

	// Create a hidden browser window
	hwnd = m_browser.Create(::GetDesktopWindow());
	ATLENSURE_RETURN(hwnd);

	// Subscribe for web actions
	m_browser.Subscribe(this);

	// Check if running for the first time
	CString sAppDir;
	bool rv = Utils::GetAppDir(sAppDir);
	ATLENSURE_RETURN(rv);
	CString sFirstRunFlag(Utils::LoadStringFromResource(IDS_FIRST_RUN_FLAG));
	CString sFirstRunPath;
	sFirstRunPath.Format(_T("%s\\%s"), sAppDir, sFirstRunFlag);
	if (ATLPath::FileExists(sFirstRunPath))
	{
		BOOL rv = ::DeleteFile(sFirstRunPath);
		ATLASSERT(rv);
		
		m_bFirstRun = true;
	}

#ifdef _DEBUG
	// Fetch notifications immediately to initialize default settings
	CheckNews();
#endif

	// Setup notification polling timer
	m_uiPollingTimerId = ::SetTimer(NULL, 0, POLLING_INTERVAL, PollingTimerProc);
	ATLENSURE_RETURN(m_uiPollingTimerId);

	return S_OK;
}

// -----------------------------------------------------------------------------
// CAtlExeModuleT
HRESULT CBrowserModule::PostMessageLoop() throw()
{
	if (m_uiPollingTimerId)
	{
		BOOL rv = ::KillTimer(NULL, m_uiPollingTimerId);
		ATLASSERT(rv);
		m_uiPollingTimerId = 0;
	}

	if (m_uiMonitorTimerId)
	{
		BOOL rv = ::KillTimer(NULL, m_uiMonitorTimerId);
		ATLASSERT(rv);
		m_uiMonitorTimerId = 0;
	}

	if (m_uiFadeInTimerId)
	{
		BOOL rv = ::KillTimer(NULL, m_uiFadeInTimerId);
		ATLASSERT(rv);
		m_uiFadeInTimerId = 0;
	}

	return S_OK;
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayOpen()
{
	CString sHomepageUrl = Utils::LoadStringFromResource(IDS_HOMEPAGE_URL);

	// Open URL in Chromatic, if it's installed, or in default browser otherwise
	CString sChromatic;
	if (Utils::GetChromaticImagePath(sChromatic))
		Utils::StartProcess(sChromatic, sHomepageUrl);
	else
		Utils::OpenUrl(sHomepageUrl);
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayCheckNews()
{
	CheckNews(true);
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTraySettings()
{
	if (!m_settingsWindow.IsWindowVisible())
		m_settingsWindow.ShowWindow(SW_SHOW);
	else
		m_settingsWindow.BringWindowToTop();
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayHelp()
{
	CString sHelpUrl = Utils::LoadStringFromResource(IDS_HELP_URL);

	// Open URL in Chromatic, if it's installed, or in default browser otherwise
	CString sChromatic;
	if (Utils::GetChromaticImagePath(sChromatic))
		Utils::StartProcess(sChromatic, sHelpUrl);
	else
		Utils::OpenUrl(sHelpUrl);
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayDisable()
{
	m_settings.disabled = !m_settings.disabled;
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayAbout()
{
	CString sAboutUrl = Utils::LoadStringFromResource(IDS_ABOUT_URL);

	// Open URL in Chromatic, if it's installed, or in default browser otherwise
	CString sChromatic;
	if (Utils::GetChromaticImagePath(sChromatic))
		Utils::StartProcess(sChromatic, sAboutUrl);
	else
		Utils::OpenUrl(sAboutUrl);
}

// -----------------------------------------------------------------------------
// ITrayEvents
void CBrowserModule::OnTrayExit()
{
	// Show confirmation message
	if (m_bQuitConfirmationPending)
		return;

	m_bQuitConfirmationPending = true;
	
	int iConfirmation = ::MessageBox(
		m_trayWindow.m_hWnd, 
		Utils::LoadStringFromResource(IDS_QUIT_CONFIRMATION), 
		Utils::LoadStringFromResource(IDS_APP_NAME), 
		MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
	if (IDYES == iConfirmation)
	{
		BOOL rv = m_browser.DestroyWindow();
		ATLASSERT(rv);

		rv = m_settingsWindow.DestroyWindow();
		ATLASSERT(rv);

		rv = m_trayWindow.DestroyWindow();
		ATLASSERT(rv);

		::PostQuitMessage(0);
	}

	m_bQuitConfirmationPending = false;
}

// -----------------------------------------------------------------------------
// IWebActions
void CBrowserModule::OnActionClose()
{
	m_browser.ShowWindow(SW_HIDE);
	m_browser.Navigate(Utils::LoadStringFromResource(IDS_BLANK_URL));
}

// -----------------------------------------------------------------------------
// IWebActions
void CBrowserModule::OnActionShow()
{
	// Make window see-through, show it and fade it in
	m_browser.SetWindowOpacity(0);
	m_browser.ShowWindow(SW_SHOW);

	m_uiFadeInTimerId = ::SetTimer(NULL, 0, FADE_IN_INTERVAL, FadeInTimerProc);
	ATLENSURE_RETURN_VOID(m_uiFadeInTimerId);
}

// -----------------------------------------------------------------------------
// IWebActions
void CBrowserModule::OnActionSettings()
{
	m_settingsWindow.ShowWindow(SW_SHOW);
	::SetForegroundWindow(m_settingsWindow);
}

// -----------------------------------------------------------------------------
// IWebActions
void CBrowserModule::OnActionExpand(const CString& _url)
{
	// Open URL in Chromatic, if it's installed, or in default browser otherwise
	CString sChromatic;
	if (Utils::GetChromaticImagePath(sChromatic))
		Utils::StartProcess(sChromatic, _url);
	else
		Utils::OpenUrl(_url);
}

// -----------------------------------------------------------------------------
// IWebActions
void CBrowserModule::OnActionSetParam(const CString& _key, const CString& _value)
{
	// Setting types are redundant and are being discarded
	// 

	// Load setting types
	CString sCatDefault(Utils::LoadStringFromResource(IDS_REMOTE_SETTING_DEFAULT));

	// Parse setting types
	CString sSettingCategory;
	if (0 == ::StrCmpNI(_key, sCatDefault, sCatDefault.GetLength()))
		sSettingCategory = sCatDefault;

	// Parse key (discard the setting type as we handle this internally)
	CString sKey(_key.Mid(sSettingCategory.GetLength()));

	// Load supported settings
	CString sLocation(Utils::LoadStringFromResource(IDS_REMOTE_SETTING_LOCATION));
	CString sCategories(Utils::LoadStringFromResource(IDS_SETTING_CATEGORIES));

	// Parse settings
	if (0 == ::StrCmpNI(sKey, sLocation, sLocation.GetLength()))
		m_settings.location = _value;
	else if (0 == ::StrCmpNI(sKey, sCategories, sCategories.GetLength()))
		m_settings.categories = m_settings.CategoriesToBitmask(_value);
}

// -----------------------------------------------------------------------------
void CBrowserModule::CheckNews(bool _onDemand)
{
	// Ensure the notification window is not already visible
	if (m_browser.IsWindowVisible())
		return;

	// Get random number for cache busting
	int iRand = rand();

	// Get client ID (MD5 hash)
	CString sClientId = CA2T(Utils::BytesToHexString(Utils::GetMd5Hash((LPSTR)CT2A(Utils::GetMachineGuid()))));

	// Get notification URL
	CString sNotificationUrl(Utils::LoadStringFromResource(IDS_NOTIFICATION_URL));

	// Get application ID
	CString sAppId(Utils::LoadStringFromResource(IDS_APP_ID));

	// Compose a URL and issue the request
	CString sUrl;
	sUrl.Format(_T("%s?emid=%s&appid=%s&rand=%d"), sNotificationUrl, sClientId, sAppId, iRand);

	if (m_bFirstRun)
	{
		m_bFirstRun = false;
		sUrl += _T("&first_run=1");
	}

	if (_onDemand)
		sUrl += _T("&manual=1");

	// Location and categories may be empty but a server bug prevents notifications to be
	// returned if either one of these arguments is sent empty, without a value.
	// Bellow is a workaround. We only add this arguments if they are not empty.
	//

	// Get country code
	CString sLocation(m_settings.location);
	if (!sLocation.IsEmpty())
		sUrl += _T("&country=") + sLocation;
	
	// Get selected categories
	CString sCategories(m_settings.CategoriesToString(m_settings.categories));
	if (!sCategories.IsEmpty())
		sUrl += _T("&categories=") + sCategories;

#ifdef _DEBUG
	sUrl += _T("&debug=1");
#endif

	m_browser.Navigate(sUrl);
}

// -----------------------------------------------------------------------------
/*static*/ VOID CALLBACK CBrowserModule::PollingTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CBrowserModule* pThis(CBrowserModule::GetInstance());
	ATLASSERT(pThis);

	if (idEvent == pThis->m_uiMonitorTimerId)
	{
		DWORD dwIdleDelta = (dwTime - pThis->m_dwMonitorStartTime);
		if (dwIdleDelta > IDLE_DURATION)
		{
			BOOL rv = ::KillTimer(NULL, pThis->m_uiMonitorTimerId);
			ATLASSERT(rv);
			pThis->m_uiMonitorTimerId = 0;

			// Monitor has expired but user is still idle
			ATLTRACE2(atlTraceUI, 0, _T("User still idle: stopping monitor"));
			return;
		}
	}

	// If notifications are enabled and notifications window is hidden, 
	// check user activity and show a notification window when user is active.
	if (!pThis->m_settings.disabled && !pThis->m_browser.IsWindowVisible())
	{
		LASTINPUTINFO lii = {0};
		lii.cbSize = sizeof(LASTINPUTINFO);
		BOOL rv = ::GetLastInputInfo(&lii);
		ATLENSURE_RETURN_VOID(rv);

		DWORD dwIdleTime = (dwTime - lii.dwTime);
		if (dwIdleTime < IDLE_DURATION)
		{
			if (pThis->m_uiMonitorTimerId)
			{
				rv = ::KillTimer(NULL, pThis->m_uiMonitorTimerId);
				ATLASSERT(rv);
				pThis->m_uiMonitorTimerId = 0;
			}

			pThis->CheckNews();
		}
		else if (!pThis->m_uiMonitorTimerId)
		{
			ATLTRACE2(atlTraceUI, 0, _T("User is idle: starting monitor"));

			// Monitor input (do not block the UI thread)
			pThis->m_uiMonitorTimerId = ::SetTimer(NULL, 0, MONITOR_INTERVAL, PollingTimerProc);
			ATLENSURE_RETURN_VOID(pThis->m_uiMonitorTimerId);

			pThis->m_dwMonitorStartTime = dwTime;
		}
	}
}

// -----------------------------------------------------------------------------
/*static*/ VOID CALLBACK CBrowserModule::FadeInTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CBrowserModule* pThis(CBrowserModule::GetInstance());
	ATLASSERT(pThis);

	if (idEvent == pThis->m_uiFadeInTimerId)
	{
		static const FLOAT fMaxAlpha = 0.9f;

		// Get current window opacity
		FLOAT fAlpha = pThis->m_browser.GetWindowOpacity();
		if (fAlpha < fMaxAlpha)
		{
			// Normalize animation step opacity range [0.0, 1.0]
			FLOAT fNormalStep = (FLOAT)FADE_IN_INTERVAL / FADE_IN_DURATION;

			// Set new window opacity
			fAlpha += fNormalStep;
			if (fAlpha > fMaxAlpha)
				fAlpha = fMaxAlpha;
			pThis->m_browser.SetWindowOpacity(fAlpha);
		}
		else
		{
			BOOL rv = ::KillTimer(NULL, pThis->m_uiFadeInTimerId);
			ATLASSERT(rv);
			pThis->m_uiFadeInTimerId = 0;
		}
	}
}
