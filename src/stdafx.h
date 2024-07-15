// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"
#include "resource.h"

// Windows
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")

// CRT
#include <time.h>

// ATL
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <atlutil.h>
#include <atlpath.h>
#include <atlfile.h>
#include <atlimage.h>
#include <atliface.h>
#include <atlsecurity.h>
#include <atlsimpcoll.h>
using namespace ATL;

// WTL
#include <atlapp.h>
#include <atlctrls.h>

// STD
#include <memory>

// C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning(disable:4290)

// Common controls
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Macros
#ifndef ATLENSURE_RETURN_VOID
#define ATLENSURE_RETURN_VOID(expr)           \
	do {                                      \
		int __atl_condVal=!!(expr);           \
		ATLASSERT(__atl_condVal);             \
		if(!(__atl_condVal)) return;          \
	} while(0) 
#endif // ATLENSURE_RETURN_VOID
