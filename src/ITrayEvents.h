#pragma once

class ITrayEvents
{
public:
	virtual void OnTrayOpen() PURE;
	virtual void OnTrayCheckNews() PURE;
	virtual void OnTraySettings() PURE;
	virtual void OnTrayHelp() PURE;
	virtual void OnTrayDisable() PURE;
	virtual void OnTrayAbout() PURE;
	virtual void OnTrayExit() PURE;
};