#include "StdAfx.h"
#include "LocalSettingsIni.h"

// ----------------------------------------------------------------------------
CLocalSettingsIni::CLocalSettingsIni(const CString& _settingsFilePath)
{
	m_sSettingsFilePath = _settingsFilePath;
	m_sAppName.LoadString(IDS_APP_NAME);
}

// ----------------------------------------------------------------------------
CLocalSettingsIni::~CLocalSettingsIni(void)
{
}

// ----------------------------------------------------------------------------
BOOL CLocalSettingsIni::GetBOOL(const CString& _key)
{
	static enum { CFG_VAL_LEN = 2 };
	TCHAR szVal[CFG_VAL_LEN + sizeof(TCHAR)] = {0};
	::GetPrivateProfileString(
		m_sAppName, 
		_key, 
		NULL, 
		szVal, 
		CFG_VAL_LEN, 
		m_sSettingsFilePath);
	return (szVal && szVal[0] == _T('1'));
}

// ----------------------------------------------------------------------------
void CLocalSettingsIni::PutBOOL(const CString& _key, BOOL _val)
{
	BOOL rv = ::WritePrivateProfileString(
		m_sAppName,
		_key, 
		(_val ? _T("1") : NULL),
		m_sSettingsFilePath);
	ATLASSERT(rv);
}

// ----------------------------------------------------------------------------
CString CLocalSettingsIni::GetString(const CString& _key)
{
	static enum { CFG_VAL_LEN = 1024 };
	TCHAR szVal[CFG_VAL_LEN + sizeof(TCHAR)] = {0};
	::GetPrivateProfileString(
		m_sAppName, 
		_key,
		NULL, 
		szVal, 
		CFG_VAL_LEN, 
		m_sSettingsFilePath);
	return (szVal ? szVal : _T(""));
}

// ----------------------------------------------------------------------------
void CLocalSettingsIni::PutString(const CString& _key, const CString& _val)
{
	BOOL rv = ::WritePrivateProfileString(
		m_sAppName, 
		_key, 
		(_val.GetLength() ? _val : NULL),
		m_sSettingsFilePath);
	ATLASSERT(rv);
}

// ----------------------------------------------------------------------------
DWORD CLocalSettingsIni::GetDWORD(const CString& _key)
{
	// Reserve enough memory for "0x" representation
	static enum { CFG_VAL_LEN = sizeof(DWORD) * 2 + 2 };
	TCHAR szVal[CFG_VAL_LEN + sizeof(TCHAR)] = {0};
	::GetPrivateProfileString(
		m_sAppName, 
		_key,
		NULL, 
		szVal, 
		CFG_VAL_LEN, 
		m_sSettingsFilePath);
	if (szVal)
	{
		DWORD dwVal = _tcstol(szVal, NULL, 16);
		ATLASSERT(ERANGE != errno);
		return dwVal;
	}

	return 0;
}

// ----------------------------------------------------------------------------
void CLocalSettingsIni::PutDWORD(const CString& _key, DWORD _val)
{
	CString sBuf;
	sBuf.Format(_T("0x%x"), _val);
	BOOL rv = ::WritePrivateProfileString(
		m_sAppName, 
		_key, 
		(_val ? sBuf : NULL), 
		m_sSettingsFilePath);
	ATLASSERT(rv);
}
