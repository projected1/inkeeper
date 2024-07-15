#pragma once

class CLocalSettingsIni
{
public:
	CLocalSettingsIni(const CString& _settingsFilePath);
	virtual ~CLocalSettingsIni(void);

	BOOL GetBOOL(const CString& _key);
	void PutBOOL(const CString& _key, BOOL _val);

	CString GetString(const CString& _key);
	void PutString(const CString& _key, const CString& _val);

	DWORD GetDWORD(const CString& _key);
	void PutDWORD(const CString& _key, DWORD _val);

protected:
	CString m_sAppName;
	CString m_sSettingsFilePath;
};
