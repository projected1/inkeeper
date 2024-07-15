// BrowserUIHandler.h : Declaration of the CBrowserUIHandler

#pragma once
#include "Browser_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CBrowserUIHandler

class ATL_NO_VTABLE CBrowserUIHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBrowserUIHandler, &CLSID_BrowserUIHandler>,
	public IDispatchImpl<IBrowserUIHandler, &IID_IBrowserUIHandler, &LIBID_BrowserLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_APPUIHANDLER)

	BEGIN_COM_MAP(CBrowserUIHandler)
		COM_INTERFACE_ENTRY(IBrowserUIHandler)
		COM_INTERFACE_ENTRY(IDocHostUIHandlerDispatch)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	CBrowserUIHandler();
	virtual ~CBrowserUIHandler();

	HRESULT FinalConstruct();
	void FinalRelease();

	// IDocHostUIHandlerDispatch
	STDMETHOD(ShowContextMenu)(DWORD, DWORD, DWORD, IUnknown*, IDispatch*, HRESULT*);
	STDMETHOD(GetHostInfo)(DWORD*, DWORD*);
	STDMETHOD(ShowUI)(DWORD, IUnknown*, IUnknown*, IUnknown*, IUnknown*, HRESULT*);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(VARIANT_BOOL);
	STDMETHOD(OnDocWindowActivate)(VARIANT_BOOL);
	STDMETHOD(OnFrameWindowActivate)(VARIANT_BOOL);
	STDMETHOD(ResizeBorder)(long, long, long, long ,IUnknown*, VARIANT_BOOL);
	STDMETHOD(TranslateAccelerator)(DWORD_PTR, DWORD, DWORD_PTR, DWORD_PTR, BSTR, DWORD, HRESULT*);
	STDMETHOD(GetOptionKeyPath)(BSTR*, DWORD);
	STDMETHOD(GetDropTarget)(IUnknown*, IUnknown**);
	STDMETHOD(GetExternal)(IDispatch**);
	STDMETHOD(TranslateUrl)(DWORD, BSTR, BSTR*);
	STDMETHOD(FilterDataObject)(IUnknown*, IUnknown**);
};

OBJECT_ENTRY_AUTO(__uuidof(BrowserUIHandler), CBrowserUIHandler)
