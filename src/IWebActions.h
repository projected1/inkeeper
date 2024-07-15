#pragma once

class IWebActions
{
public:
	virtual void OnActionClose() PURE;
	virtual void OnActionShow() PURE;
	virtual void OnActionSettings() PURE;
	virtual void OnActionExpand(const CString& _url) PURE;
	virtual void OnActionSetParam(const CString& _key, const CString& _value) PURE;
};