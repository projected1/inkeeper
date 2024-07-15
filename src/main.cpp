#include "stdafx.h"
#include "BrowserModule.h"
#include "Utils.h"


// -----------------------------------------------------------------------------
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, 
								HINSTANCE hPrevInstance, 
								LPTSTR lpCmdLine, 
								int nShowCmd)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	// Enable run-time memory check in debug builds
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Allow only single app instance to run at a time
	CString sMutexGuid = Utils::LoadStringFromResource(IDS_MUTEX_GUID);
	CHandle hMutex(::CreateMutex(NULL, TRUE, sMutexGuid));
	ATLENSURE_RETURN_VAL(hMutex, 1);
	DWORD dwError = ::GetLastError();
	if (ERROR_ALREADY_EXISTS == dwError)
		return 1;

	// Go
	CBrowserModule* pApp = CBrowserModule::GetInstance();
	return pApp->WinMain(nShowCmd);
}

