#pragma once
#include "BrowserHost.h"
#include "IWebActions.h"

#include <exdispid.h> // for |DIID_DWebBrowserEvents2| |DISPID_XXX|

class CAppBrowser :
	public CBrowserHost,
	public IDispEventImpl<1, CAppBrowser, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1> // |DispEventAdvise|
{
public:
	BEGIN_MSG_MAP(CAppBrowser)
		CHAIN_MSG_MAP(CBrowserHost)
	END_MSG_MAP()
	
	BEGIN_SINK_MAP(CAppBrowser)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR, OnNavigateError)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW3, OnNewWindow3)
	END_SINK_MAP()

public:
	CAppBrowser(void);
	virtual ~CAppBrowser(void);

	void Navigate(const CString& _url);
	void Subscribe(IWebActions* _listener);

protected:
	// CComObjectRootEx
	HRESULT FinalConstruct();
	void FinalRelease();

	// WebBrowserEvents2
	STDMETHOD_(void, OnDocumentComplete)(IDispatch*, VARIANT*);
	STDMETHOD_(void, OnBeforeNavigate2)(IDispatch*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT_BOOL*);
	STDMETHOD_(void, OnNavigateError)(IDispatch*, VARIANT*, VARIANT*, VARIANT*, VARIANT_BOOL*);
	STDMETHOD_(void, OnNewWindow3)(IDispatch**, VARIANT_BOOL*, DWORD, BSTR, BSTR);

private:
	bool HandleWebAction(IDispatch* pDisp, VARIANT* URL);

	// Helper methods
	void SubclassBrowser();

	// Callbacks
	static BOOL CALLBACK EnumChildBrowser(HWND hwnd, LPARAM lParam);
	static LRESULT CALLBACK BrowserWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	bool m_bAdvised; // For |DWebBrowserEvents2|
	HWND m_hwndBrowser;
	CSimpleArray<IWebActions*> m_aEventListeners;

private:
	static WNDPROC m_wndprocBrowser;
};
