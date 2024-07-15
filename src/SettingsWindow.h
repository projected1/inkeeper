#pragma once
#include "Settings.h"

class CSettingsWindow :
	public CDialogImpl<CSettingsWindow>
{
public:
	enum { IDD = IDD_SETTINGS_DIALOG };

	enum 
	{ 
		WND_STYLE = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED,
		WND_EX_STYLE = WS_EX_PALETTEWINDOW
	};

	BEGIN_MSG_MAP(CSettingsWindow)
		// Messages
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)

		// Commands
		COMMAND_HANDLER(IDC_BUTTON_OK, BN_CLICKED, OnButtonClickedOk)
		COMMAND_HANDLER(IDC_BUTTON_CANCEL, BN_CLICKED, OnButtonClickedCancel)
	END_MSG_MAP()

public:
	CSettingsWindow(CSettings* _settings);
	virtual ~CSettingsWindow(void);

protected:
	// Message handlers
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);

	// Command handlers
	LRESULT OnButtonClickedOk(WORD, WORD, HWND, BOOL&);
	LRESULT OnButtonClickedCancel(WORD, WORD, HWND, BOOL&);

private:
	void Serialize();
	void Unserialize();

private:
	// Types
	typedef CWinTraits<WND_STYLE, WND_EX_STYLE> CSettingsWinTraits;

protected:
	CSettings* m_pSettings;
	CSimpleMap<CString, UINT> m_mapLocationCtrls;
};
