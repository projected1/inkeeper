#pragma once
#include "AppBrowser.h"

class CAppWindow :
	public CAppBrowser
{
protected:
	enum { WM_MOUSEENTER = (WM_USER + __COUNTER__) };

	enum 
	{
		WND_WIDTH = 483,
		WND_HEIGHT = 101,

		ALPHA_TRANSPARENT = 0,
		ALPHA_OPAQUE = 255
	};

public:
	BEGIN_MSG_MAP(CAppWindow)
		// Messages
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEENTER, OnMouseEnter)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)

		// Chaining
		CHAIN_MSG_MAP(CAppBrowser)
	END_MSG_MAP()

public:
	CAppWindow(void);
	virtual ~CAppWindow(void);

	FLOAT GetWindowOpacity();
	void SetWindowOpacity(FLOAT _alpha);

protected:
	// Message handlers
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseEnter(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL&);

protected:
	bool m_bMouseHover;
};