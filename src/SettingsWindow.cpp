#include "stdafx.h"
#include "Utils.h"
#include "SettingsWindow.h"

// -----------------------------------------------------------------------------
CSettingsWindow::CSettingsWindow(CSettings* _settings)
{
	ATLASSERT(_settings);
	m_pSettings = _settings;

	// Init location controls mapping
	m_mapLocationCtrls.Add(Utils::LoadStringFromResource(IDS_COUNTRY_CODE_US), IDC_RADIO_LOC_US);
	m_mapLocationCtrls.Add(Utils::LoadStringFromResource(IDS_COUNTRY_CODE_FRANCE), IDC_RADIO_LOC_FRANCE);
	m_mapLocationCtrls.Add(Utils::LoadStringFromResource(IDS_COUNTRY_CODE_GERMANY), IDC_RADIO_LOC_GERMANY);
	m_mapLocationCtrls.Add(Utils::LoadStringFromResource(IDS_COUNTRY_CODE_ISRAEL), IDC_RADIO_LOC_ISRAEL);
}

// -----------------------------------------------------------------------------
CSettingsWindow::~CSettingsWindow(void)
{
}

// -----------------------------------------------------------------------------
LRESULT CSettingsWindow::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Set window icons
	CIcon icon;
	HICON hIcon = icon.LoadIcon(IDI_APP_ICON);
	ATLENSURE_RETURN_VAL(hIcon, -1);
	this->SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	this->SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	// Set window position
	BOOL rv = this->CenterWindow();
	ATLENSURE_RETURN_VAL(rv, -1);

	// Set window caption
	CString sDefaultWindowCaption;
	this->GetWindowText(sDefaultWindowCaption);
	CString sAppName(Utils::LoadStringFromResource(IDS_APP_NAME));
	CString sWindowCaption;
	sWindowCaption.Format(_T("%s %s"), sAppName, sDefaultWindowCaption);
	this->SetWindowText(sWindowCaption);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
LRESULT CSettingsWindow::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Load settings if window is being shown
	if ((BOOL)wParam == TRUE)
		Unserialize();

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
LRESULT CSettingsWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(SW_HIDE);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CSettingsWindow::OnButtonClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// Serialize changes and hide this window
	Serialize();
	this->PostMessage(WM_CLOSE);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
// Command handler
LRESULT CSettingsWindow::OnButtonClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// Discard changed and hide this window
	this->PostMessage(WM_CLOSE);

	bHandled = TRUE;
	return 0;
}

// ----------------------------------------------------------------------------
void CSettingsWindow::Serialize()
{
	// Enabled / disabled
	CButton btn(this->GetDlgItem(IDC_RADIO_DISBALED));
	int iChecked = btn.GetCheck();
	m_pSettings->disabled = (BST_CHECKED == iChecked);
	btn.Detach();

	// Subscription categories
	DWORD dwCategories = 0;

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_NEWS));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_NEWS;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_WORLD));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_WORLD;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_TECH));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_TECH;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_SPORTS));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_SPORTS;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_BUSINESS));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_BUSINESS;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_HEALTH));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_HEALTH;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_ENTERTAINMENT));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_ENTERTAINMENT;
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_SCIENCE));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		dwCategories |= CSettings::CAT_SCIENCE;
	btn.Detach();

	m_pSettings->categories = dwCategories;

	// Location
	btn.Attach(this->GetDlgItem(IDC_RADIO_LOC_US));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		m_pSettings->location = Utils::LoadStringFromResource(IDS_COUNTRY_CODE_US);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_RADIO_LOC_FRANCE));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		m_pSettings->location = Utils::LoadStringFromResource(IDS_COUNTRY_CODE_FRANCE);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_RADIO_LOC_GERMANY));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		m_pSettings->location = Utils::LoadStringFromResource(IDS_COUNTRY_CODE_GERMANY);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_RADIO_LOC_ISRAEL));
	iChecked = btn.GetCheck();
	if (BST_CHECKED == iChecked)
		m_pSettings->location = Utils::LoadStringFromResource(IDS_COUNTRY_CODE_ISRAEL);
	btn.Detach();
}

// ----------------------------------------------------------------------------
void CSettingsWindow::Unserialize()
{
	// Enabled / disabled
	DWORD dwCtrlId = 
		m_pSettings->disabled ? IDC_RADIO_DISBALED : IDC_RADIO_ENABLED;
	BOOL rv = this->CheckRadioButton(IDC_RADIO_ENABLED, IDC_RADIO_DISBALED, dwCtrlId);
	ATLASSERT(rv);

	// Subscription categories
	DWORD dwCategories = m_pSettings->categories;

	CButton btn(this->GetDlgItem(IDC_CHECK_CAT_NEWS));
	DWORD dwCheckAction = 
		(dwCategories & CSettings::CAT_NEWS) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_WORLD));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_WORLD) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_TECH));
	dwCheckAction = (dwCategories & CSettings::CAT_TECH) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_SPORTS));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_SPORTS) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_BUSINESS));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_BUSINESS) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_HEALTH));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_HEALTH) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_ENTERTAINMENT));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_ENTERTAINMENT) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	btn.Attach(this->GetDlgItem(IDC_CHECK_CAT_SCIENCE));
	dwCheckAction = 
		(dwCategories & CSettings::CAT_SCIENCE) ? BST_CHECKED : BST_UNCHECKED;
	btn.SetCheck(dwCheckAction);
	btn.Detach();

	// Location
	CString sCurLocation(m_pSettings->location);
	int iIdx = m_mapLocationCtrls.FindKey(sCurLocation);
	if (-1 != iIdx)
	{
		dwCtrlId = m_mapLocationCtrls.GetValueAt(iIdx);
		BOOL rv = this->CheckRadioButton(IDC_RADIO_LOC_US, IDC_RADIO_LOC_ISRAEL, dwCtrlId);
		ATLASSERT(rv);
	}
}
