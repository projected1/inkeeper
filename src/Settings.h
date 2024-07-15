#pragma once
#include "LocalSettingsIni.h"

class CSettings :
	public CLocalSettingsIni
{
public:
	// Categories bit-mask
	enum
	{
		CAT_ALL = 0xff,
		CAT_NEWS = 0x1,
		CAT_WORLD = 0x2,
		CAT_TECH = 0x4,
		CAT_SPORTS = 0x8,
		CAT_BUSINESS = 0x10,
		CAT_HEALTH = 0x20,
		CAT_ENTERTAINMENT = 0x40,
		CAT_SCIENCE = 0x80
	};

	// Locations enumeration
	enum
	{
		LOC_ERROR = 0,
		LOC_US = 1,
		LOC_FRANCE = 2,
		LOC_GERMANY = 3,
		LOC_ISRAEL = 4
	};

public:
	CSettings(void);
	virtual ~CSettings(void);

	// Properties (API)
	__declspec(property(get = GetDisabled, put = PutDisabled)) BOOL disabled;
	__declspec(property(get = GetLocation, put = PutLocation)) CString location;
	__declspec(property(get = GetCategories, put = PutCategories)) DWORD categories;

	// Getters and setters
	BOOL GetDisabled();
	void PutDisabled(BOOL _val);

	CString GetLocation();
	void PutLocation(const CString& _val);

	DWORD GetCategories();
	void PutCategories(DWORD _val);

	// Helper methods (settings utils)
	CString CategoriesToString(DWORD _categories);
	DWORD CategoriesToBitmask(const CString& _categories);

private:
	void InitDefaultSettings();
	CString GetSettingsFilePath();

protected:
	// TODO: Consider overriding |CSimpleMapEqualHelper| to provide case insensitive comparison
	CSimpleMap<DWORD, CString> m_mapLocations;
	CSimpleMap<CString, DWORD> m_mapCategories;
};
