/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o., Nova Dubnica, Slovakia,  				   *
**																							   *
************************************************************************************************
**																							   *
** Project: OpcDbGateway.exe		Project name:  OpcDbGateway     			               *
**																							   *
************************************************************************************************
** @doc EXTERNAL 
** @module Example1.h | Example external DLL.
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

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "CS.h"
#include "ProcessImageMemory.h"

const UINT LOGGING_LEVEL_SWITCH1 = 1029;//MO LoggL1 - used to define logging level from configuration - can be used for whole class when set in constructor
const UINT LOGGING_LEVEL_SWITCH2 = 1030;//MO LoggL2 - used to define logging level from configuration - can be used to reduce logging level in a member function

// CExample1App
// See Example1.cpp for the implementation of this class
//

class CExample1App : public CWinApp
{
public:
	CExample1App();

// Overrides
public:
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;

	void Clear();

	//DECLARE_MESSAGE_MAP()
public:
	CCS					m_cCS;
	CProcessImageMemory	*m_pProcessImageMemory;
	HANDLE				m_hStopWaitEvent;
	HANDLE				m_hIOThread;
	LPSTR				m_pszCmdline; // Command line for child process
	BOOL				m_bInitialisedFromDoProcessIO;
	_bstr_t				m_bstr_tCmdline;
	DWORD				m_dwPvIndexForStdOut;
	DWORD				m_dwPvIndexForNewStdOutNotification;
};

extern CExample1App theApp;