/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o.Nova Dubnica, Slovakia,  				   *
**																							   *
************************************************************************************************
**																							   *
** Project: OpcDbGateway.exe		Project name:  OpcDbGateway     			               *
**																							   *
************************************************************************************************
** @doc EXTERNAL 
** @module Example1.cpp | Example external DLL.
**
************************************************************************************************
** @info
** Version			: 2.0<nl>
** Compiler			: MSVC 7.0<nl>
** Author			: SAE - Automation, s.r.o.<nl>
************************************************************************************************
** @history
** 03.06.2011	Initial version<nl>
***********************************************************************************************/

#include "stdafx.h"
#include "ExternalDllInterface.h"
#include "Example1.h"

#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include "ChildStdoutReader.h"
#include "Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Max length of the CLI string
#define MAXCLISTRING 300
//
#define IS_VALID_MO_ADDRESS(address) ((address != (DWORD)-1) && (address != 0))
#define IS_VALID_USER_MO_ADDRESS(address) (IS_VALID_MO_ADDRESS(address) && (address > 100))

// CExample1App

//BEGIN_MESSAGE_MAP(CExample1App, CWinApp)
//END_MESSAGE_MAP()


// CExample1App construction
CExample1App::CExample1App()
{
	m_pProcessImageMemory = NULL;
	m_hIOThread = NULL;
	m_hStopWaitEvent = NULL;
	m_bInitialisedFromDoProcessIO = false;
	m_pszCmdline = NULL;
}

// The one and only CExample1App object
CExample1App theApp;

// CExample1App initialization
BOOL CExample1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CExample1App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bSleep = false;
	
	m_cCS.Enter();

	if (m_hStopWaitEvent != NULL)
	{
		SetEvent(m_hStopWaitEvent);
		bSleep = true;
	}

	m_cCS.Leave();

	if (bSleep)
		Sleep(200);

	Clear();

	if (!theApp.m_pszCmdline) {
		delete theApp.m_pszCmdline;
		theApp.m_pszCmdline = NULL;
	}

	return CWinApp::ExitInstance();
}

void CExample1App::Clear()
{
	m_cCS.Enter();

	if (m_hStopWaitEvent != NULL)
	{
		CloseHandle(m_hStopWaitEvent);
		m_hStopWaitEvent = NULL;
	}

	if (m_hIOThread)
	{
		CloseHandle(m_hIOThread);
		m_hIOThread = NULL;
	}
	
	theApp.m_pProcessImageMemory = NULL;

	m_cCS.Leave();
}

DWORD WINAPI IOThread(LPVOID pParam)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());// Probably not necessary as it is not exported function
	//Sleep(10000); // only to atach by debugging
	TRY
	{
		//Test if parameters from DoProcessIO are prepared
		while (!theApp.m_bInitialisedFromDoProcessIO) {
			Sleep(100);
		}
		CLogger logger;

		logger.InitLogger(theApp.m_pProcessImageMemory, LOGING_LEVEL_ALL/*, LOGGING_LEVEL_SWITCH1*/); // logging level can be changed in different methods later 
		HRESULT hr;
		CComVariant cVariantEmptyMsg;
		cVariantEmptyMsg.ChangeType(VT_BSTR);
		cVariantEmptyMsg.bstrVal = _bstr_t(_T(""));

		while (S_OK != (hr = theApp.m_pProcessImageMemory->Write(CProcessImageMemory::SYSADDR_LOG_MSG_STR, cVariantEmptyMsg, OPC_QUALITY_GOOD))) // Vycistime log
		{
			Sleep(100);
		}
		logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - dll started"));
				
		ChildStdoutReader stdr;
		if ( ! stdr.CreateChildProcess(theApp.m_pszCmdline) ){
	
			//End of dll thread if child process cannot be created
			logger.LogMsg(LOGING_LEVEL_ALL, _T("E:childprocessreader - The child process cannot be created"));
			if (! theApp.m_pszCmdline) {
				delete theApp.m_pszCmdline; //TODO dostane sa tu?
				theApp.m_pszCmdline = NULL;
			}
			theApp.Clear();
			return 0;
		}

		logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - child process is running"));
		if (! theApp.m_pszCmdline) {
			delete theApp.m_pszCmdline;
			theApp.m_pszCmdline = NULL;
		}
		while(1)
		{
			if (WAIT_OBJECT_0 == ::WaitForSingleObject(theApp.m_hStopWaitEvent, 1000)) {
				if (!stdr.TerminateChildProcess()) {
					logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - Child process could not be terminated"));
					break;
				}
				logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - Stop event received"));
				break;
			}
			
			// Read from stdout only if stop from DoProcessIO has not arrived otherway wait on new allowance
			// to continue. Evaluation of Stop Event is not blocked this way
			if (theApp.m_pProcessImageMemory != NULL && theApp.m_bInitialisedFromDoProcessIO)
			{ 
				BOOL bSuccess = stdr.ReadFromPipe();
				if (!bSuccess || stdr.dwRead == 0) {
					logger.LogMsg(LOGING_LEVEL_ALL, _T("E: wrong reading from pipe or dwRead == 0 "));
					break;
				}
				stdr.chBuf[stdr.dwRead] = 0x00;
				logger.LogMsg(LOGING_LEVEL_INFORMATION, stdr.chBuf);
				
				//Notify that new value of stdout arrived if notifications activated
				//New string will be written to PV only if previous notification has been
				//confirmed from runtime by setting PV for notification to false.
				//Handshake control by notofications will not be activated if
				//theApp.m_dwPvIndexForNewStdOutNotification is set to 0
				CComVariant CV(stdr.chBuf);
				CV.vt = VT_BSTR;
				if (theApp.m_dwPvIndexForNewStdOutNotification != 0) { // notifications activated
					CComVariant CV2(true);
					CV2.vt = VT_BOOL;
					CProcessValue PV;
					theApp.m_pProcessImageMemory->Read(theApp.m_dwPvIndexForNewStdOutNotification, &PV);
					if (! PV.vValue.boolVal) { // if notification from runtime confirmed
						//write to child stdout to PV
						theApp.m_pProcessImageMemory->Write(theApp.m_dwPvIndexForStdOut, CV);
						//set notification variable
						theApp.m_pProcessImageMemory->Write(theApp.m_dwPvIndexForNewStdOutNotification, CV2);
					}
				}
				else { // notifications not activated
					//Write new string from child process stdout to process variable
					theApp.m_pProcessImageMemory->Write(theApp.m_dwPvIndexForStdOut, CV);
				}
				
				//Terminate process if STOP from broker // for other child prcesses 
				//it will be ignored
				if (! strcmp("STOP\r\n", stdr.chBuf)) {
					logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - STOP from broker"));
					if (!stdr.TerminateChildProcess()) {
						logger.LogMsg(LOGING_LEVEL_ALL, _T("E:childprocessreader - problem by TerminateChildProcess"));
						break;
					}
				}

				
				//TODO write buffer content to MO

				// find out if it is valid JSON string
				// parse JSON
				// actualise related memory operands
				
				/*
				// Example 1: How to Read a item value.
				CProcessValue PV;
				if (SUCCEEDED(theApp.m_pProcessImageMemory->Read(CProcessImageMemory::SYSADDR_PLC_PERIOD_COUNTER, &PV)))
				{
					ATLTRACE(_T("\nDLL Example 1: Read value(%d)=%u\n"), CProcessImageMemory::SYSADDR_PLC_PERIOD_COUNTER, PV.vValue.ulVal);

					// Example 2: How to Write a new item value.
					if (SUCCEEDED(theApp.m_pProcessImageMemory->Write(5001, PV)))
						ATLTRACE(_T("\nDLL Example 2: Write value(%d)=%u\n"), 5001, PV.vValue.ulVal);

					// Example 3: How to Write a new item value.
					CComVariant CV(PV.vValue.ulVal + 10);
					if (SUCCEEDED(theApp.m_pProcessImageMemory->Write(5002, CV)))
						ATLTRACE(_T("\nDLL Example 3: Write value(%d)=%u\n"), 5002, CV.ulVal);
				}

				
				*/
			}
		}
		if (stdr.TerminateChildProcess() )
			logger.LogMsg(LOGING_LEVEL_ALL, _T("I:childprocessreader - child process terminated\n"));
		theApp.Clear(); //after receiving Stop event to end the main thread of Dll
	}
	CATCH_ALL(e)
	{
		if (e)
		{
			//ATLTRACE("Dll ChildStdoutReader: Exception ocured\n");
			e->Delete();
			theApp.Clear();
		}
	}
	END_CATCH_ALL;

	return 0;
}

//---------------------------------------------------------------------------------------------------------
// BEGIN - DLL MANDATORY INTERFACE
//---------------------------------------------------------------------------------------------------------

//*********************************************************************************************************
// @mfunc   bool | OnStart | This function is called after main process application thread is sucessfully started.
//
// @rdesc   Returns void
//
// @comm    You can use this function to perform tasks such as allocating resources used by DLL.
//
// @history 02.06.2011 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport void WINAPI OnStart()
{	
	theApp.m_hStopWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	DWORD dwThreadId;
	theApp.m_hIOThread = CreateThread(NULL, 0, IOThread, &theApp, 0, &dwThreadId);

	//////////////////////////////////////////////////////TODO
	/*theApp.m_pszCmdline = new TCHAR[MAXCLISTRING];
	if (theApp.m_pszCmdline)
		theApp.m_pszCmdline[MAXCLISTRING] = 0x00;
	
	TCHAR pokus[] = TEXT("mosquitto_sub.exe -h sae33 -t topic2");
		
	int length;
	_tcscpy_s(theApp.m_pszCmdline, 10, _T("Hello"));

	_tcsncpy_s((TCHAR)(&theApp.m_pszCmdline[MAXCLISTRING]), _T("Hello"), 10);
	_tcsncpy_s<MAXCLISTRING>(theApp.m_pszCmdline, _tcslen(pokus), pokus);
		_tcsncpy_s<MAXCLISTRING>(

	if ( (length = _tcslen(pokus) ) < MAXCLISTRING)
		_tcsncpy_s(theApp.m_pszCmdline, pokus, _tcslen(pokus));
	else
		_tcscpy_s(theApp.m_pszCmdline, MAXCLISTRING - 1, pokus);
	*/
}

//*********************************************************************************************************
// @mfunc   bool | OnStop | This function is called after main process application thread is sucessfully stopped.
//
// @rdesc   Returns void
//
// @comm    You can use this function to perform tasks such as releasing resources used by DLL.
//
// @history 02.06.2011 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport void WINAPI OnStop()
{	
	if (theApp.m_hStopWaitEvent != INVALID_HANDLE_VALUE)
		SetEvent(theApp.m_hStopWaitEvent);
}

//*********************************************************************************************************
// @mfunc   bool | OnInitMemory | The function is called after the main process application memry is sucessfully initialized.
//
// @parm    CProcessImageMemory* | lpPIM  | [in] Pointer to the buffer that inludes input values.
//
// @rdesc   Returns void
//
// @comm    The function is called after the main process application memry is sucessfully initialized.
//
// @history 02.06.2011 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport void WINAPI OnInitMemory(CProcessImageMemory* lpPIM)
{		
	// DO NOT CHANGE THIS FUNCTION 
	theApp.m_pProcessImageMemory = lpPIM;
}

//*********************************************************************************************************
// @mfunc   DWORD | GetProductName | The function retrieves a DLL product name as string.
//
// @parm    LPTSTR             | lpReturnedString  | [out] Pointer to the buffer that receives the retrieved string. 
// @parm    DWORD              | dwSize            | [in] Size of the buffer pointed to by the lpReturnedString parameter, in TCHARs
//
// @rdesc   The return value is the number of characters copied to the buffer, not including the terminating null character.
//
//          If the supplied destination buffer is too small to hold the requested string, 
//          the string is truncated and followed by a null character, and the return value is equal to nSize minus one.
//
// @comm    The function retrieves a DLL product name as string.
//
//          The function will be called from OpcDbGateway configurator module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport DWORD WINAPI GetProductName(LPTSTR lpReturnedString, DWORD dwSize)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	s.LoadString(IDS_PRODUCT_NAME);
	_tcscpy_s(lpReturnedString, dwSize, s.GetBuffer());
	return (DWORD)s.GetLength();
}

//*********************************************************************************************************
// @mfunc   DWORD | GetProductVersion | The function retrieves a DLL product version as string.
//
// @parm    LPTSTR             | lpReturnedString  | [out] Pointer to the buffer that receives the retrieved string. 
// @parm    DWORD              | dwSize            | [in] Size of the buffer pointed to by the lpReturnedString parameter, in TCHARs
//
// @rdesc   The return value is the number of characters copied to the buffer, not including the terminating null character.
//
//          If the supplied destination buffer is too small to hold the requested string, 
//          the string is truncated and followed by a null character, and the return value is equal to nSize minus one.
//
// @comm    The function retrieves a DLL product version as string.
//
//          The function will be called from OpcDbGateway configurator module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport DWORD WINAPI GetProductVersion(LPTSTR lpReturnedString, DWORD dwSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	s.LoadString(IDS_PRODUCT_VERSION);
	_tcscpy_s(lpReturnedString, dwSize, s.GetBuffer());
	return (DWORD)s.GetLength();
}

//*********************************************************************************************************
// @mfunc   DWORD | GetCompanyName | The function retrieves a DLL company name as string.
//
// @parm    LPTSTR             | lpReturnedString  | [out] Pointer to the buffer that receives the retrieved string. 
// @parm    DWORD              | dwSize            | [in] Size of the buffer pointed to by the lpReturnedString parameter, in TCHARs
//
// @rdesc   The return value is the number of characters copied to the buffer, not including the terminating null character.
//
//          If the supplied destination buffer is too small to hold the requested string, 
//          the string is truncated and followed by a null character, and the return value is equal to nSize minus one.
//
// @comm    The function retrieves a DLL company name as string.
//
//          The function will be called from OpcDbGateway configurator module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport DWORD WINAPI GetCompanyName(LPTSTR lpReturnedString, DWORD dwSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	s.LoadString(IDS_COMPANY_NAME);
	_tcscpy_s(lpReturnedString, dwSize, s.GetBuffer());
	return (DWORD)s.GetLength();
}

//*********************************************************************************************************
// @mfunc   DWORD | GetLegalCopyright | The function retrieves a DLL copyright as string.
//
// @parm    LPTSTR             | lpReturnedString  | [out]Pointer to the buffer that receives the retrieved string. 
// @parm    DWORD              | dwSize            | [in] Size of the buffer pointed to by the lpReturnedString parameter, in TCHARs
//
// @rdesc   The return value is the number of characters copied to the buffer, not including the terminating null character.
//
//          If the supplied destination buffer is too small to hold the requested string, 
//          the string is truncated and followed by a null character, and the return value is equal to nSize minus one.
//
// @comm    The function retrieves a DLL copyright as string.
//
//          The function will be called from OpcDbGateway configurator module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport DWORD WINAPI GetLegalCopyright(LPTSTR lpReturnedString, DWORD dwSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	s.LoadString(IDS_LEGAL_COPYRIGHT);
	_tcscpy_s(lpReturnedString, dwSize, s.GetBuffer());
	return (DWORD)s.GetLength();
}

//*********************************************************************************************************
// @mfunc   DWORD | GetDescription | The function retrieves a DLL description as string.
//
// @parm    LPTSTR             | lpReturnedString  | [out]Pointer to the buffer that receives the retrieved string. 
// @parm    DWORD              | dwSize            | [in] Size of the buffer pointed to by the lpReturnedString parameter, in TCHARs
//
// @rdesc   The return value is the number of characters copied to the buffer, not including the terminating null character.
//
//          If the supplied destination buffer is too small to hold the requested string, 
//          the string is truncated and followed by a null character, and the return value is equal to nSize minus one.
//
// @comm    The function retrieves a DLL description as string.
//
//          The function will be called from OpcDbGateway configurator module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport DWORD WINAPI GetDescription(LPTSTR lpReturnedString, DWORD dwSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	s.LoadString(IDS_PRODUCT_DESCRIPTION);
	_tcscpy_s(lpReturnedString, dwSize, s.GetBuffer());
	return (DWORD)s.GetLength();
}

//*********************************************************************************************************
// @mfunc   void | GetCountOfIO | The function retrieves a specified count of inputs/outputs.
//
// @parm    LPWORD             | lpInputs  | [in, out] Pointer to count of input values.
// @parm    LPWORD             | lpOutputs | [in, out] Pointer to count of output values.
//
// @rdesc   Returns always void.
//
// @comm    The function retrieves a specified count of inputs/outputs.
//
//          The function will be called from OpcDbGateway configurator module and runtime module.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport void WINAPI GetCountOfIO(LPWORD lpInputs, LPWORD lpOutputs)
{
	*lpInputs  = 4;			// TODO: Set your count of inputs
	*lpOutputs = 1;			// TODO: Set your count of outputs
	/*
	Inputs
	const CComVariant *lpActualStatus = lpInputs; //Status 1 - Start thread 2 - Stop thread
	const CComVariant *lpCliString = lpInputs +1; // Command line interface string for child process
	const CComVariant *lpPVIndexForChildStdoutString = lpInputs + 2; //Index of PV for placing stdout string from child process
	const CComVariant *lpPvIndexForNewStdOutNotification = lpInputs + 3; //Index of PV for placing stdout string from child process
	Output
	CComVariant *lpLogFromDoProcessIO = lpOutputs; //Output error / info from DoProcessIO call
	*/
}

//*********************************************************************************************************
// @mfunc   bool | DoProcessIO | The function processes all values from input buffer and retrieves the processed values in output buffer.
//
// @parm    const CComVariant* | lpInputs  | [in] Pointer to the buffer that inludes input values.
// @parm    WORD               | wInCnt    | [in] Size of the buffer pointed to by the lpInputs parameter, in CComVariant
// @parm    CComVariant*       | lpOutputs | [out]Pointer to the buffer that recieves output values.
// @parm    WORD               | wOutCnt   | [in] Size of the buffer pointed to by the lpOutputs parameter, in CComVariant
//
// @rdesc   Returns one of the following values:
//
// @flag    true  | Successfully
// @flag    false | Error value otherwise
//
// @comm    The function processes all values from input buffer and retrieves the processed values in output buffer.
//
//          The function will be called from OpcDbGateway runtime module as Function Blocks/Command.
//
// @history 24.01.2008 13:11, created by SAE - Automation, s.r.o., 
//*********************************************************************************************************
DllExport bool WINAPI DoProcessIO(const CComVariant* lpInputs, WORD wInCnt, 
								        CComVariant* lpOutputs, WORD wOutCnt)
{
	
	//Test nr. input and output parameters
	if (lpInputs == NULL || wInCnt < 4 || wOutCnt < 1)
		return false;
	/*
	if(lpInputs + 3 != lpOutputs)
		return false;
		Unfortunatelly, they are not in the same array but PVs have to bee
	*/
	const CComVariant *lpActualStatus = lpInputs; //Status 1 - Start thread 2 - Stop thread
	const CComVariant *lpCliString = lpInputs +1; // Command line interface string for child process
	const CComVariant *lpPVIndexForChildStdoutString = lpInputs + 2; //Index of PV for placing stdout string from child process
	const CComVariant *lpPvIndexForNewStdOutNotification = lpInputs + 3; //Index of PV for placing stdout string from child process
	CComVariant *lpLogFromDoProcessIO = lpOutputs; //Output error / info from DoProcessIO call
	
	/*
	lpOutputs is not initialised before calling DoProcessIO.
	Because of this next test would ever return false

	if (lpLogFromDoProcessIO->vt != VT_BSTR) //Test parameter for log string first to be able to explain error reasons
	{
		lpLogFromDoProcessIO->vt = VT_BSTR;
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: Wrong type for LogFromDoProcessIO"));
		return false;
	}
	*/
	
	lpLogFromDoProcessIO->vt = VT_BSTR; //It is not initialised before calling

	if (lpActualStatus->vt != VT_UI2) {
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: Wrong type for MoActualStatus"));
		return false;
	}
	if (lpActualStatus->iVal < 1 || lpActualStatus->iVal > 2)  {//Test acceptable  of parameter for cli string
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: Wrong status value"));
		return false;
	}

	if (lpCliString->vt != VT_BSTR) { //Test type of parameter for CLI string
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: Wrong type for CLI string"));
		return false;
	}
	if (_bstr_t(lpCliString->bstrVal).length() == 0) { //Test type of parameter for length of cli string
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: CLI string has length 0"));
		return false;
	}
	if (lpPVIndexForChildStdoutString->vt != VT_UI2) {
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: Wrong type for PVIndexForChildStdoutString"));
		return false;
	}
	
	if (!theApp.m_pProcessImageMemory) {
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: None direct access to process image memory"));
		return false;
	}

	//Test PV indexes
	if(! IS_VALID_USER_MO_ADDRESS(lpPVIndexForChildStdoutString->iVal))
	{
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOInfo: Notifications will not be activated"));
		return false;
	}
	//Notifications will be activted only if PvIndexForNewStdOutNotification !=0 and is valid
	if (lpPvIndexForNewStdOutNotification->iVal == 0) {
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: PvIndexForNewStdOutNotification out of limits"));
	}
		
	else if(!IS_VALID_USER_MO_ADDRESS(lpPvIndexForNewStdOutNotification->iVal))
	{
		*lpLogFromDoProcessIO = (LPCTSTR)_bstr_t(_T("DoProcesIOErr: PvIndexForNewStdOutNotification out of limits"));
		return false;
	}
	
	//Set CLI string for child process using _bstrt_t 
	theApp.m_bstr_tCmdline = lpCliString;
	theApp.m_pszCmdline = theApp.m_bstr_tCmdline;

	//Set PV index for stdout 
	theApp.m_dwPvIndexForStdOut = lpPVIndexForChildStdoutString->iVal;
	//Set PV index for notification of new value
	theApp.m_dwPvIndexForNewStdOutNotification = lpPvIndexForNewStdOutNotification->iVal;
	
	//TODO other tests

	
	// To inform about setting up of the parameters or stop the continous working thread
	if (lpActualStatus->iVal == 1)
	{
		theApp.m_bInitialisedFromDoProcessIO = TRUE;
	}
	else if (lpActualStatus->iVal == 2)
	{
		theApp.m_bInitialisedFromDoProcessIO = FALSE;
	}
	


	/*
	// TODO: Add your functionality

	// EXAMPLE: Output[i] = Inputs[i] + 1 
	// BEGIN {
	for(DWORD i=0; i < wInCnt; i++)
	{
		CComVariant value = lpInputs[i];
		value.ChangeType(VT_I4);
		
		ATLTRACE("\nDLL: i:%d VARTYPE=%d Value=%d", i, value.vt, value.lVal);
		lpOutputs[i] = value.lVal + 1; 
	}
	// } END
	*/
	return true;
}

//---------------------------------------------------------------------------------------------------------
// END - DLL MANDATORY INTERFACE
//---------------------------------------------------------------------------------------------------------
