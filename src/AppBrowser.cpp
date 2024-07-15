#include "stdafx.h"
#include "Utils.h"
#include "AppBrowser.h"

// MSHTML
#include <mshtml.h>
#import <mshtml.tlb> no_auto_exclude auto_rename named_guids
typedef MSHTML::IHTMLDocument3Ptr IHTMLDocument3Ptr;

/*static*/ WNDPROC CAppBrowser::m_wndprocBrowser = NULL;

// -----------------------------------------------------------------------------
CAppBrowser::CAppBrowser(void)
{
	m_bAdvised = false;
	m_hwndBrowser = NULL;
}

// -----------------------------------------------------------------------------
CAppBrowser::~CAppBrowser(void)
{
}

// -----------------------------------------------------------------------------
HRESULT CAppBrowser::FinalConstruct()
{
	return S_OK;
}

// -----------------------------------------------------------------------------
void CAppBrowser::FinalRelease()
{
	if (m_bAdvised)
	{
		HRESULT hr = this->DispEventUnadvise(m_spWebBrowser);
		ATLENSURE_RETURN_VOID(SUCCEEDED(hr));
		m_bAdvised = false;
	}
}

// -----------------------------------------------------------------------------
void CAppBrowser::Navigate(const CString& _url)
{
	ATLTRACE2(atlTraceUI, 0, _url);

	HRESULT hr = S_OK;

	// We must subscribe here because |FinalConstruct| is called before |WM_CREATE|
	// is processed, and the browser is only created in the |WM_CREATE| handler.
	if (!m_bAdvised)
	{
		hr = this->DispEventAdvise(m_spWebBrowser);
		ATLENSURE_RETURN_VOID(SUCCEEDED(hr));
		m_bAdvised = true;
	}

	// Navigate
	CComBSTR bstrUrl(_url);
	hr = m_spWebBrowser->Navigate(bstrUrl);
	ATLENSURE_RETURN_VOID(SUCCEEDED(hr));
}

// -----------------------------------------------------------------------------
void CAppBrowser::Subscribe(IWebActions* _listener)
{
	ATLASSERT(_listener);
	m_aEventListeners.Add(_listener);
}

// -----------------------------------------------------------------------------
// WebBrowserEvents2
STDMETHODIMP_(void) CAppBrowser::OnDocumentComplete(
	IDispatch* pDisp, 
	VARIANT* URL)
{
	// Subclass the real browser window so we can get it's window messages
	if (!m_hwndBrowser)
		SubclassBrowser();
}

// -----------------------------------------------------------------------------
// WebBrowserEvents2
STDMETHODIMP_(void) CAppBrowser::OnBeforeNavigate2(
	IDispatch* pDisp, 
	VARIANT* URL,
	VARIANT* Flags,
	VARIANT* TargetFrameName,
	VARIANT* PostData,
	VARIANT* Headers,
	VARIANT_BOOL* Cancel)
{
	// Process commands that are issues via custom URL APIs
	bool bHandled = HandleWebAction(pDisp, URL);
	*Cancel = bHandled ? VARIANT_TRUE : VARIANT_FALSE;
}

// -----------------------------------------------------------------------------
// WebBrowserEvents2
STDMETHODIMP_(void) CAppBrowser::OnNavigateError(
	IDispatch* pDisp, 
	VARIANT* URL, 
	VARIANT* TargetFrameName, 
	VARIANT* StatusCode, 
	VARIANT_BOOL* Cancel)
{
	*Cancel = VARIANT_TRUE;
	ATLTRACE2(atlTraceUI, 0, _T("Navigation error: %d: %s "), StatusCode->intVal, URL->bstrVal);
}

// -----------------------------------------------------------------------------
// WebBrowserEvents2
STDMETHODIMP_(void) CAppBrowser::OnNewWindow3(
	IDispatch** ppDisp, 
	VARIANT_BOOL* Cancel,
	DWORD dwFlags,
	BSTR bstrUrlContext,
	BSTR bstrUrl)
{
	// Block pop-up windows, which will otherwise open in Internet Explorer
	*Cancel = VARIANT_TRUE;
}

// -----------------------------------------------------------------------------
bool CAppBrowser::HandleWebAction(IDispatch* pDisp, VARIANT* URL)
{
	// Check if URL is a valid web-action.
	// A valid web-action is any URL that conforms to the "action://" API 
	// specification or a URL that is not a notification URL. The later is 
	// equivalent to "action://expand/".
	CString sActionScheme(Utils::LoadStringFromResource(IDS_ACTION_SCHEME));
	CString sNotificationUrl(Utils::LoadStringFromResource(IDS_NOTIFICATION_URL));
	CString sBlankUrl(Utils::LoadStringFromResource(IDS_BLANK_URL));	
	bool bAction = (0 == ::StrCmpNI(URL->bstrVal, sActionScheme, sActionScheme.GetLength()));
	bool bBlankUrl = (0 == sBlankUrl.CompareNoCase(URL->bstrVal));
	bool bNotificationUrl = (0 == ::StrCmpNI(URL->bstrVal, sNotificationUrl, sNotificationUrl.GetLength()));
	if (!bAction && (bBlankUrl || bNotificationUrl))
		return false;

	CString sWebAction(URL->bstrVal);

#ifdef _DEBUG
	CString sBuf(sWebAction);
	sBuf.Replace(_T("%"), _T("%%"));
	ATLTRACE2(atlTraceUI, 0, sBuf);
#endif

	// Parse action
	CString sActionClose(Utils::LoadStringFromResource(IDS_ACTION_CLOSE));
	CString sActionShow(Utils::LoadStringFromResource(IDS_ACTION_SHOW));
	CString sActionSettings(Utils::LoadStringFromResource(IDS_ACTION_SETTINGS));
	CString sActionExpand(Utils::LoadStringFromResource(IDS_ACTION_EXPAND));
	CString sActionSetParam(Utils::LoadStringFromResource(IDS_ACTION_SET_PARAM));

	if (0 == ::StrCmpNI(sWebAction, sActionClose, sActionClose.GetLength()))
	{
		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionClose();
	}
	else if (0 == ::StrCmpNI(sWebAction, sActionShow, sActionShow.GetLength()))
	{
		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionShow();
	}
	else if (0 == ::StrCmpNI(sWebAction, sActionSettings, sActionSettings.GetLength()))
	{
		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionSettings();
	}
	else if (0 == ::StrCmpNI(sWebAction, sActionSetParam, sActionSetParam.GetLength()))
	{
		// Parse key-value pair
		CString sPair = sWebAction.Mid(sActionSetParam.GetLength());
		int iPos = sPair.Find(_T('='));
		ATLENSURE_RETURN_VAL(-1 != iPos, true);

		CString sKey(sPair.Mid(0, iPos));
		CString sValue(sPair.Mid(iPos + 1));

		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionSetParam(sKey, sValue);
	}
	else if (0 == ::StrCmpNI(sWebAction, sActionExpand, sActionExpand.GetLength()))
	{
		// Unescape URL component
		CString sUrl = sWebAction.Mid(sActionExpand.GetLength());
		DWORD dwLen = 0;
		BOOL rv = ::AtlUnescapeUrl(sUrl, sUrl.GetBuffer(), &dwLen, sUrl.GetLength());
		ATLENSURE_RETURN_VAL(rv, true);
		ATLENSURE_RETURN_VAL(dwLen, true);

		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionExpand(sUrl);
	}
	else if (::PathIsURL(sWebAction))
	{
		for (int i = 0; i < m_aEventListeners.GetSize(); i++)
			m_aEventListeners[i]->OnActionExpand(sWebAction);
	}
	else
	{
		// Shouldn't get here (unsupported web action?)
		ATLTRACE2(atlTraceUI, 0, _T("Unsupported web action: %s"), sWebAction);

		ATLASSERT(0);
	}

	return true;
}

// ----------------------------------------------------------------------------
// Subclasses the real browser window
void CAppBrowser::SubclassBrowser()
{
	ATLASSERT(!m_hwndBrowser);

	BOOL rv = ::EnumChildWindows(this->m_hWnd, EnumChildBrowser, (LPARAM)&m_hwndBrowser);
	if (m_hwndBrowser)
	{
		m_wndprocBrowser = (WNDPROC)::SetWindowLongPtr(
			m_hwndBrowser, 
			GWLP_WNDPROC, 
			(LPARAM)(WNDPROC)BrowserWindowProc);
		ATLASSERT(m_wndprocBrowser);
	}
}

// -----------------------------------------------------------------------------
/*static*/ BOOL CALLBACK CAppBrowser::EnumChildBrowser(HWND hwnd, LPARAM lParam)
{
	static enum { MAX_CLASS_NAME_LEN = 100 };
	TCHAR szClassName[MAX_CLASS_NAME_LEN] = {0};
	::GetClassName(hwnd, szClassName, MAX_CLASS_NAME_LEN);

	CString sBrowserClassName(Utils::LoadStringFromResource(IDS_IE_CLASS_NAME));	
	if (sBrowserClassName == szClassName)
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Filters messages that root window needs
/*static*/ LRESULT CALLBACK CAppBrowser::BrowserWindowProc(
	HWND hwnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
		{
			HWND hwndRoot = ::GetAncestor(hwnd, GA_ROOT);
			ATLASSERT(hwndRoot);

			// Delegate message to root window
			BOOL rv = ::PostMessage(hwndRoot, uMsg, wParam, lParam);
			ATLASSERT(rv);
		}
		break;
	}

	return ::CallWindowProc(m_wndprocBrowser, hwnd, uMsg, wParam, lParam);
}
