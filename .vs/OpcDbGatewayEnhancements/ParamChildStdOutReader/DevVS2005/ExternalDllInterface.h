/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o., Nova Dubnica, Slovakia,  					   *
**																							   *
************************************************************************************************
**																							   *
** Project: OpcDbGateway.exe		Project name:  OpcDbGateway     			               *
**																							   *
************************************************************************************************
** @doc EXTERNAL 
** @module ExternalDllInterface.h | External DLL interface.
**
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 7.0<nl>
** Author			: SAE - Automation, s.r.o., <nl>
************************************************************************************************
** @history
** 24.01.2008	Initial version<nl>
***********************************************************************************************/

#pragma once

#define DllExport extern "C" __declspec (dllexport)
#define DllImport extern "C" __declspec (dllimport)

class CProcessImageMemory;

//////////////////////////////////////////////////////////////////////////////
// EXTERNAL DLL INTERFACE
/////////////////////////////////////////////////////////////////////////////
// The function retrieves a DLL product name as string.
DllExport DWORD WINAPI GetProductName		(LPTSTR lpReturnedString, DWORD dwSize);
// The function retrieves a DLL product version as string.
DllExport DWORD WINAPI GetProductVersion	(LPTSTR lpReturnedString, DWORD dwSize);
// The function retrieves a DLL company name as string.
DllExport DWORD WINAPI GetCompanyName		(LPTSTR lpReturnedString, DWORD dwSize);
// The function retrieves a DLL copyright as string.
DllExport DWORD WINAPI GetLegalCopyright	(LPTSTR lpReturnedString, DWORD dwSize);
// The function retrieves a DLL description as string.
DllExport DWORD WINAPI GetDescription		(LPTSTR lpReturnedString, DWORD dwSize);
// The function retrieves a specified count of inputs/outputs.
DllExport void	WINAPI GetCountOfIO			(LPWORD lpInputs, LPWORD lpOutputs);
// The function processes all values from input buffer and retrieves the processed values into output buffer.
DllExport bool	WINAPI DoProcessIO			(const CComVariant* lpInputs, WORD wInCnt,
											       CComVariant* lpOutputs, WORD wOutCnt);
// The function is called after the main process application memry is sucessfully initialized.
DllExport void  WINAPI OnInitMemory			(CProcessImageMemory* lpPIM);
// This function is called after main process application thread is sucessfully started.
DllExport void  WINAPI OnStart				();
// This function is called after main process application thread is sucessfully stopped.
DllExport void  WINAPI OnStop				();