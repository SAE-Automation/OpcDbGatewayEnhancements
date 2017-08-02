// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0501
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
//#include <afxtempl.h>
//#include <atlstr.h>

//#include <windows.h>

//#include <atlbase.h>
//extern CComModule _Module;
//#include <atlcom.h>

#include <comdef.h>

#include <vector>
#include <map>
#include <set>
#include <bitset>

#include "opcda.h"