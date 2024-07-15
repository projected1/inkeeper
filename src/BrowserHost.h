// BrowserHost.h : Declaration of the CBrowserHost

#pragma once
#include "Browser_i.h"
#include "BrowserUIHandler.h"

// Import shell explorer ("SHDocVw")
#import "progid:Shell.Explorer" raw_interfaces_only, auto_rename, no_auto_exclude 


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CBrowserHost

class ATL_NO_VTABLE CBrowserHost :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBrowserHost, &CLSID_BrowserHost>,
	public IDispatchImpl<IBrowserHost, &IID_IBrowserHost, &LIBID_BrowserLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CWindowImpl<CBrowserHost, CAxWindow2, CNullTraits> // ATL ActiveX window
{
public:
	DECLARE_WND_CLASS_EX(NULL, 0, 0)

	DECLARE_REGISTRY_RESOURCEID(IDR_BROWSERHOST)

	DECLARE_NOT_AGGREGATABLE(CBrowserHost)

	BEGIN_COM_MAP(CBrowserHost)
		COM_INTERFACE_ENTRY(IBrowserHost)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	BEGIN_MSG_MAP(CBrowserHost)
		// Messages
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	CBrowserHost(void);
	virtual ~CBrowserHost(void);

protected:
	// Message handlers
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);

protected:
	SHDocVw::IWebBrowser2Ptr m_spWebBrowser;
	CComObjectStackEx<CBrowserUIHandler> m_browserUIHandler; // Ref-counted stack object
};

OBJECT_ENTRY_AUTO(__uuidof(BrowserHost), CBrowserHost)
