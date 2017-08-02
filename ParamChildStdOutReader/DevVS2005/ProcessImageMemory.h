/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o., Nova Dubnica, Slovakia,  				   *
**																							   *
************************************************************************************************
**																							   *
** Project: OpcDbGateway.exe		Project name:  OpcDbGateway             			       *
**																							   *
************************************************************************************************
** @doc EXTERNAL 
** @module ProcessImageMemory.h | 
**
** This source file contains the implementation of the <c Class CProcessImageMemory>                   
** These classes implements .... .<nl>
**
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 6.0<nl>
** Author			: TS, SAE - Automation, s.r.o.<nl>
************************************************************************************************
** @history
** TS 24.1.2002	Initial version<nl>
***********************************************************************************************/

#pragma once
#pragma pack(push)
#pragma pack(8)

#include "ObjectError.h"
#include "ComVariantEx.h"
#include "CS.h"

class CProcessValue
{
	
public:
	CProcessValue::CProcessValue()
	{
		dwID = 0;
		ftTimeStamp.dwLowDateTime = 0;
		ftTimeStamp.dwHighDateTime = 0;
		wQuality = OPC_QUALITY_GOOD;
		wTimeStampType = TimeStampInternal;
	}
enum {
	TimeStampInternal = 1,	//timestamp created internaly by the scheduler
	TimeStampExternal = 2,	//timestamp taken from the connected opc server
};

public:
	DWORD		dwID;
	CComVariantEx	vValue;
	FILETIME	ftTimeStamp;
	WORD		wQuality;
	WORD		wTimeStampType; 
};

typedef void (*ProcessMOUpdateHandler_t)(__in DWORD VarAddress, __in CProcessValue &cValue);

class CProcessImageMemory : public CObjectError
{
public:

	enum{
		MEMORY_SIZE = 100000,		//Maximum size of the process image memory		// Ko - EC_0072
		SYSTEM_SIZE = 100,			//Maximum size of the system memory
	};

	enum {
		SYSADDR_SYNCQUEUE_SIZE = 1, //Address of the system variable SYNCQUEUE_SIZE
		SYSADDR_ASYNCQUEUE_SIZE = 2,//Address of the system variable ASYNCQUEUE_SIZE
		SYSADDR_TIME_LOG = 3,		//Address of the system variable TIME_LOG 
		                            //   this variable should be modified by an OPC client 
									//   if it is TRUE then the timemeassurements will be written to a log file for each function block
		SYSADDR_TRACE_LOG = 4,      //Addres of the system variable TRACE_LOG
									//   this variable should be modified by an OPC client 
									//   if it is TRUE then the all commands in a function block are traced and written to a log file									
		SYSADDR_PLC_PERIOD = 5,		//The period between two synchroniyation impulses - read/write
		SYSADDR_PLC_PERIOD_MEASURED = 6,			 //Measured Plc period
		SYSADDR_PLC_PERIOD_COUNTER = 7,				 //counter of Plc periods
        SYSADDR_PLC_CYCLE = 8,						 //the current length of one plc cycle
		SYSADDR_PLC_CYCLE_MAX = 9,					 //the maximum measured length of a Plc cycle
		SYSADDR_PLC_CYCLE_MIN = 10,					 //the minimum measured length of a Plc cycle
		SYSADDR_PLC_CYCLE_LIMITOVERFLOW_COUNTER = 11,//increments when the measured length of the plc cycle is greater than the length of a plc period		
		SYSADDR_PLC_SYSTEMTIME = 12,				 //the current system time
		SYSADDR_PLC_CYCLE_LIMITOVERFLOW_PERCENT = 13,//100 * PLC_CYCLE_LIMITOVERFLOW_COUNTER / PLC_PERIOD_COUNTER [%]
		SYSADDR_PLC_USED_LOGS_SPACE = 14,			//The current size of the HD space used by log files [MB]
		SYSADDR_PLC_USED_REPORTS_SPACE = 15,		//The current size of the HD space used by reports [MB]
		SYSADDR_PLC_AVAIL_VIRTUALMEMORY = 16,		//The current size of available virtual memory [MB]
		SYSADDR_PLC_USED_PROCESSDB_SPACE = 17,		//The current size of the process database [MB]
		SYSADDR_PLC_LOGS_FULL =	18,					//This variable is TRUE if (PLC_USED_ LOGS_SPACE >= MAX_LOGS_SPACE)
		SYSADDR_PLC_REPORTS_FULL = 19,				//This variable is TRUE if (PLC_USED_REPORTS_SPACE >= MAX_REPORTS_SPACE)
		SYSADDR_PLC_VIRTUALMEMORY_LOW = 20,		//This variable is TRUE if (PLC_USED_ VIRTUALMEMORY >= MAX_VIRTUALMEMORY)
		SYSADDR_PLC_PROCESSDB_FULL = 21,			//This variable is TRUE if ( PLC_USED_ PROCESSDB_SPACE >= MAX_ PROCESSDB_SPACE)
		SYSADDR_PLC_ACLineStatus = 22,	//AC power status. This parameter can be one of the following values. Value	Meaning
										//0	Offline
										//1	Online
										//255	Unknown status
		SYSADDR_PLC_BatteryFlag	= 23,	//Battery charge status. This parameter can be a combination of the following values. Value	Meaning
										//1	High
										//2	Low
										//4	Critical
										//8	Charging
										//128	No system battery
										//255	Unknown status
										//All other values are reserved.

		SYSADDR_PLC_BatteryLifePercent = 24,	//Percentage of full battery charge remaining. This member can be a value in the range 0 to 100, or 255 if status is unknown. All other values are reserved. 
		SYSADDR_PLC_BatteryLifeTime	= 25,		//Number of seconds of battery life remaining, or -1 if remaining seconds are unknown. 
		SYSADDR_PLC_BatteryFullLifeTime	= 26,	//Number of seconds of battery life when at full charge, or -1 if full battery lifetime is unknown.
		SYSADDR_PLC_STOP = 27,					//System variable PLC_STOP can be used to stop the scheduler manually. If this variable is set to TRUE either by an OPC client or from a function block then the scheduler stops its activity (and content of the process image memory is stored to the table Persist Data).
		SYSADDR_PLC_START =	28,					//A connected opc client can start the scheduler (which has been manually stopped) either by disconnecting and connecting to the scheduler again or by setting a system variable PLC_START to TRUE.
		SYSADDR_PLC_STATUS = 29,				//The current status of the scheduler:1 .. Running2 .. Stopped
		SYSADDR_PLC_STATUS_DESCRIPTION = 30,	//The string representation of the current status of the scheduler:"Running""Stopped"
		SYSADDR_PLC_STOP_REASON = 31,           //The reason why the scheduler has been stopped - "Manual", "Power failure"
		SYSADDR_PLC_STOP_REASON_DESCRIPTION = 32, //The reason why the scheduler has been stopped - "Manual", "Power failure"
		SYSADDR_PLC_GET_LAST_ERROR_DESCRIPTION = 33, //The last-error description
		SYSADDR_ACTUAL_ALARM_NAME = 34,//Name of the acknowledged alarm
		SYSADDR_GeneretedReportsDatabase = 35,  
		SYSADDR_GeneretedReportsTable = 36,  
		SYSADDR_NotPrintedReportsDatabase = 37,  
		SYSADDR_NotPrintedReportsTable = 38,  
		SYSADDR_NewGeneratedReport = 39,  
		SYSADDR_ReportOperator = 40,  
		SYSADDR_ReportName = 41,  
		SYSADDR_PrintCommand = 42,  
		SYSADDR_PrintStatus = 43,  
		SYSADDR_PrintConfirm = 44,
		SYSADDR_ActuaAlarmStatus= 46,//actual alarm status 
		SYSADDR_ActualAlarmAck=47,//command to acknowledge actual alarm from client
		SYSADDR_NextAlarm=48,//to take next alarm in vector as actual 
		SYSADDR_PreviousAlarm=49,//to take previous alarm in vector as actual 
		SYSADDR_AlarmDelete=50,//delete the alarm will be set 
		SYSADDR_AlarmLanguage=51,//delete the alarm will be set 
		SYSADDR_ActualAlarmIndex = 52,//index aktuálneho alarmu
		SYSADDR_AlarmStatusDatabase = 53,  // AlarmStatus - Process database
		SYSADDR_AlarmStatusTable = 54,     // AlarmStatus - Process table
		SYSADDR_AlarmStatusOperator = 55,  // AlarmStatus - Operator (OPCClient.AlarmViewer)
		SYSADDR_ActualAlarmCommentary = 56,// Commentary to the actual alarm
		SYSADDR_LOCALTIME = 57,			   // The current local date and time
		SYSADDR_LOG_MSG_STR = 58,		   // String that is saved to Log file, when SYSADDR_LOG_MSG_TRIGGER is set to TRUE.
		SYSADDR_LOG_MSG_TRIGGER = 59,	   // See SYSADDR_LOG_MSG_STR.
	};

public:
	HRESULT Read(DWORD VarAddress, DWORD BitNr, CProcessValue* pValue);
	HRESULT Write(DWORD VarAddress, DWORD BitNr, CProcessValue& Value);
	HRESULT Write(DWORD dwAddress, const CComVariant& vValue, WORD wQuality = OPC_QUALITY_GOOD);
	HRESULT Write(DWORD dwAddress, const CComVariant& vValue, const FILETIME &ftTimeStamp, WORD wQuality = OPC_QUALITY_GOOD);
	DWORD GetMaxAddress();
	CProcessImageMemory();
	virtual ~CProcessImageMemory();
	static BOOL IsReadOnlySystemAddress(DWORD dwAddress)
	{
		if (dwAddress >= SYSADDR_PLC_PERIOD && dwAddress < SYSADDR_PLC_STOP)
			return TRUE;
		if (dwAddress >= SYSADDR_PLC_STATUS && dwAddress < SYSADDR_ACTUAL_ALARM_NAME)
			return TRUE;
		if (dwAddress == SYSADDR_LOCALTIME)
			return TRUE;
		return FALSE;
	}
	
	HRESULT Init(const FILETIME &ActualTime, DWORD dwMaxAddress = MEMORY_SIZE);
	HRESULT Clear();
	HRESULT Write(std::map<DWORD, CProcessValue> &cValueMap);
	HRESULT Write(DWORD VarAddress, CProcessValue& Value);
	HRESULT Read(DWORD VarAddress, CProcessValue* pValue);
	void AddUpdateHandler(ProcessMOUpdateHandler_t pHandler);
	void RemoveUpdateHandler(ProcessMOUpdateHandler_t pHandler);

protected:

	HRESULT WriteInternal(DWORD VarAddress, CProcessValue &Value);

	CCS				m_cs;															// Ko - EC_0060
	CProcessValue** m_pProcMem;
	DWORD m_dwMaxAddress;
	std::vector<ProcessMOUpdateHandler_t>	m_cUpdateHandlerVector;
};

//extern CProcessImageMemory processImageMemory_g;
#pragma pack(pop)