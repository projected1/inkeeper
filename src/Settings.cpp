#include "stdafx.h"
#include "Utils.h"
#include "Settings.h"

// ----------------------------------------------------------------------------
CSettings::CSettings(void)
:CLocalSettingsIni(GetSettingsFilePath())
{
	// Init internal locations mapping
	m_mapLocations.Add(LOC_US, Utils::LoadStringFromResource(IDS_COUNTRY_CODE_US));
	m_mapLocations.Add(LOC_FRANCE, Utils::LoadStringFromResource(IDS_COUNTRY_CODE_FRANCE));
	m_mapLocations.Add(LOC_GERMANY, Utils::LoadStringFromResource(IDS_COUNTRY_CODE_GERMANY));
	m_mapLocations.Add(LOC_ISRAEL, Utils::LoadStringFromResource(IDS_COUNTRY_CODE_ISRAEL));

	// Init internal categories mapping
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_WORLD), CAT_WORLD);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_NEWS), CAT_NEWS);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_TECH), CAT_TECH);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_SPORTS), CAT_SPORTS);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_BUSINESS), CAT_BUSINESS);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_HEALTH), CAT_HEALTH);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_ENTERTAINMENT), CAT_ENTERTAINMENT);
	m_mapCategories.Add(Utils::LoadStringFromResource(IDS_CAT_SCIENCE), CAT_SCIENCE);

	// Init default settings if no settings exist yet
	if (!ATLPath::FileExists(m_sSettingsFilePath))
		InitDefaultSettings();
}

// ----------------------------------------------------------------------------
CSettings::~CSettings(void)
{
}

// ----------------------------------------------------------------------------
void CSettings::InitDefaultSettings()
{
	// Create empty settings file (overwrite existing file if exists)
	CAtlFile settingsFile;
	HRESULT hr = settingsFile.Create(
		m_sSettingsFilePath, 
		0,
		0,
		CREATE_ALWAYS);
	ATLENSURE_RETURN_VOID(SUCCEEDED(hr));

	// Init default settings
	disabled = FALSE;

	// Deprecated. Categories are initialized by the server
	//categories = CAT_ALL;

	// Deprecated. Location is initialized by the server
	//location = Utils::LoadStringFromResource(IDS_COUNTRY_CODE_ISRAEL);
}

// ----------------------------------------------------------------------------
BOOL CSettings::GetDisabled()
{
	return GetBOOL(Utils::LoadStringFromResource(IDS_SETTING_DISABLED));
}

// ----------------------------------------------------------------------------
void CSettings::PutDisabled(BOOL _val)
{
	PutBOOL(Utils::LoadStringFromResource(IDS_SETTING_DISABLED), _val);
}

// ----------------------------------------------------------------------------
CString CSettings::GetLocation()
{
	CString sLocation;

	DWORD dwLocationId = GetDWORD(Utils::LoadStringFromResource(IDS_SETTING_LOCATION));
	int iIdx = m_mapLocations.FindKey(dwLocationId);
	if (-1 != iIdx)
	{
		// Success
		sLocation = m_mapLocations.GetValueAt(iIdx);
	}

	return sLocation;
}

// ----------------------------------------------------------------------------
void CSettings::PutLocation(const CString& _val)
{
	int iIdx = m_mapLocations.FindVal(_val);
	ATLENSURE_RETURN_VOID(-1 != iIdx);

	DWORD dwLocationId = m_mapLocations.GetKeyAt(iIdx);
	PutDWORD(Utils::LoadStringFromResource(IDS_SETTING_LOCATION), dwLocationId);
}

// ----------------------------------------------------------------------------
DWORD CSettings::GetCategories()
{
	return GetDWORD(Utils::LoadStringFromResource(IDS_SETTING_CATEGORIES));
}

// ----------------------------------------------------------------------------
void CSettings::PutCategories(DWORD _val)
{
	PutDWORD(Utils::LoadStringFromResource(IDS_SETTING_CATEGORIES), _val);
}

// ----------------------------------------------------------------------------
CString CSettings::GetSettingsFilePath()
{
	CString sAppDir;
	bool rv = Utils::GetAppDir(sAppDir);
	ATLENSURE_RETURN_VAL(rv, _T(""));

	// Get path to app settings
	CString sSettingsFile(Utils::LoadStringFromResource(IDS_SETTINGS_FILE));
	CString sSettingsFilePath;
	sSettingsFilePath.Format(_T("%s\\%s"), sAppDir, sSettingsFile);
	ATLTRACE2(atlTraceUI, 0, sSettingsFilePath);

	return sSettingsFilePath;
}

// ----------------------------------------------------------------------------
CString CSettings::CategoriesToString(DWORD _categories)
{
	if (!_categories)
		return _T("");

	CString sDelimiter(Utils::LoadStringFromResource(IDS_CAT_STRING_DELIMITER));
	CString sCategories;

	if (_categories & CSettings::CAT_WORLD)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_WORLD);
	if (_categories & CSettings::CAT_NEWS)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_NEWS);
	if (_categories & CSettings::CAT_TECH)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_TECH);
	if (_categories & CSettings::CAT_SPORTS)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_SPORTS);
	if (_categories & CSettings::CAT_BUSINESS)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_BUSINESS);
	if (_categories & CSettings::CAT_HEALTH)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_HEALTH);
	if (_categories & CSettings::CAT_ENTERTAINMENT)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_ENTERTAINMENT);
	if (_categories & CSettings::CAT_SCIENCE)
		sCategories += sDelimiter + Utils::LoadStringFromResource(IDS_CAT_SCIENCE);

	// Truncate the leading comma delimiter
	if (!sCategories.IsEmpty())
		sCategories = sCategories.Mid(1);

	return sCategories;
}

// ----------------------------------------------------------------------------
DWORD CSettings::CategoriesToBitmask(const CString& _categories)
{
	if (_categories.IsEmpty())
		return 0;

	DWORD dwCategory = 0;
	DWORD dwCategories = 0;

	CString sDelimiter(Utils::LoadStringFromResource(IDS_CAT_STRING_DELIMITER));
	int iPos = 0;
	CString sToken = _categories.Tokenize(sDelimiter, iPos);
	while (!sToken.IsEmpty())
	{
		// Parse token
		dwCategory = m_mapCategories.Lookup(sToken);
		ATLENSURE_RETURN_VAL(dwCategory, 0);

		dwCategories |= dwCategory;

		// Get next token
		sToken = _categories.Tokenize(sDelimiter, iPos);
	}

	return dwCategories;
}
