#include "stdafx.h"
#include "BrowserHost.h"

// -----------------------------------------------------------------------------
CBrowserHost::CBrowserHost(void)
{
}

// -----------------------------------------------------------------------------
CBrowserHost::~CBrowserHost(void)
{
	while (m_browserUIHandler.m_dwRef)
		m_browserUIHandler.Release();
}

// -----------------------------------------------------------------------------
LRESULT CBrowserHost::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HRESULT hr = this->CreateControlLic(OLESTR("Shell.Explorer"));
	ATLENSURE_RETURN_VAL(SUCCEEDED(hr), -1);

	hr = this->QueryControl(&m_spWebBrowser);
	ATLENSURE_RETURN_VAL(SUCCEEDED(hr), -1);
	ATLENSURE_RETURN_VAL(m_spWebBrowser, -1);

	hr = this->SetExternalUIHandler(&m_browserUIHandler);
	ATLENSURE_RETURN_VAL(SUCCEEDED(hr), -1);

#ifndef _DEBUG
	// Deprecate message boxes
	hr = m_spWebBrowser->put_Silent(VARIANT_TRUE);
	ATLENSURE_RETURN_VAL(SUCCEEDED(hr), -1);
#endif

	bHandled = FALSE;
	return 0;
}
