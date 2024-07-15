// BrowserUIHandler.cpp : Implementation of CBrowserUIHandler

#include "stdafx.h"
#include "BrowserUIHandler.h"


// CBrowserUIHandler

// ----------------------------------------------------------------------------
CBrowserUIHandler::CBrowserUIHandler()
{
}

// ----------------------------------------------------------------------------
CBrowserUIHandler::~CBrowserUIHandler()
{
}

// ----------------------------------------------------------------------------
HRESULT CBrowserUIHandler::FinalConstruct()
{
	return S_OK;
}

// ----------------------------------------------------------------------------
void CBrowserUIHandler::FinalRelease()
{
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::ShowContextMenu( 
	/* [in] */ DWORD dwID,
	/* [in] */ DWORD x,
	/* [in] */ DWORD y,
	/* [in] */ IUnknown *pcmdtReserved,
	/* [in] */ IDispatch *pdispReserved,
	/* [retval][out] */ HRESULT *dwRetVal)
{
	// Deprecate the default context menu
	*dwRetVal = S_OK;
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::GetHostInfo( 
	/* [out][in] */ DWORD *pdwFlags,
	/* [out][in] */ DWORD *pdwDoubleClick)
{
	// Disable 3D border
	*pdwFlags = *pdwFlags | DOCHOSTUIFLAG_NO3DBORDER; 
	return S_OK;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::ShowUI( 
	/* [in] */ DWORD dwID,
	/* [in] */ IUnknown *pActiveObject,
	/* [in] */ IUnknown *pCommandTarget,
	/* [in] */ IUnknown *pFrame,
	/* [in] */ IUnknown *pDoc,
	/* [retval][out] */ HRESULT *dwRetVal)
{
	// TODO: To avoid a memory leak, forward calls to IDocHostUIHandler::ShowUI to the default UI handler.
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::HideUI(void)
{
	// TODO: To avoid a memory leak, forward calls to IDocHostUIHandler::ShowUI to the default UI handler.
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::UpdateUI(void)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::EnableModeless( 
	/* [in] */ VARIANT_BOOL fEnable)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::OnDocWindowActivate( 
	/* [in] */ VARIANT_BOOL fActivate)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::OnFrameWindowActivate( 
	/* [in] */ VARIANT_BOOL fActivate)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::ResizeBorder( 
	/* [in] */ long left,
	/* [in] */ long top,
	/* [in] */ long right,
	/* [in] */ long bottom,
	/* [in] */ IUnknown *pUIWindow,
	/* [in] */ VARIANT_BOOL fFrameWindow)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::TranslateAccelerator( 
	/* [in] */ DWORD_PTR hWnd,
	/* [in] */ DWORD nMessage,
	/* [in] */ DWORD_PTR wParam,
	/* [in] */ DWORD_PTR lParam,
	/* [in] */ BSTR bstrGuidCmdGroup,
	/* [in] */ DWORD nCmdID,
	/* [retval][out] */ HRESULT *dwRetVal)
{
	// The message was not handled. Host default behavior is allowed.
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::GetOptionKeyPath( 
	/* [out] */ BSTR *pbstrKey,
	/* [in] */ DWORD dw)
{
	pbstrKey = NULL;
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::GetDropTarget( 
	/* [in] */ IUnknown *pDropTarget,
	/* [out] */ IUnknown **ppDropTarget)
{
	return E_NOTIMPL;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::GetExternal( 
	/* [out] */ IDispatch **ppDispatch)
{
	*ppDispatch = NULL;
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::TranslateUrl( 
	/* [in] */ DWORD dwTranslate,
	/* [in] */ BSTR bstrURLIn,
	/* [out] */ BSTR *pbstrURLOut)
{
	*pbstrURLOut = NULL;
	return S_FALSE;
}

// ----------------------------------------------------------------------------
// IDocHostUIHandlerDispatch
STDMETHODIMP CBrowserUIHandler::FilterDataObject( 
	/* [in] */ IUnknown *pDO,
	/* [out] */ IUnknown **ppDORet)
{
	*ppDORet = NULL;
	return S_FALSE;
}
