#pragma once

#include "ProcessImageMemory.h"

typedef enum _LOGING_LEVEL // within called logging functions the actual logging level must be higher than actually accepted logging level
{
   LOGING_LEVEL_NOTHING = 0, // none logging will be done
   LOGING_LEVEL_ERROR = 50,
   LOGING_LEVEL_WARNING = 100,
   LOGING_LEVEL_INFORMATION = 200 ,
   LOGING_LEVEL_ALL = 300
} LOGING_LEVEL;

class CLogger
{
public:
	CLogger::CLogger();
	~CLogger(void);

	bool InitLogger(CProcessImageMemory	*pProcessImageMemory, LOGING_LEVEL m_LogingLevel, DWORD	m_dwLogParameterAddrValue = (DWORD)-1);//It must be called before the 1st calling of LogMsg(...) 
	void LogMsg(__in LOGING_LEVEL Level, __in const CString &szMsg);
	void LogMsg(__in LOGING_LEVEL Level, __in UINT nMsgID);
	void LogMsg1(__in LOGING_LEVEL Level, __in UINT nMsgID, __in DWORD dwValue);
	void LogMsg1(__in LOGING_LEVEL Level, __in UINT nMsgID, __in const CString &szValue);
	HRESULT CLogger::ClearLogMsg(void);// will be probably removed
	bool CanWriteToLog(void);// will be probably removed

	

protected:
	bool CLogger::ReadConfiguredLoggingLevel(__in DWORD dwLogParameterAddrValue);// Reading of logging level from PIM 
	LOGING_LEVEL   m_LogingLevel;// reference logging level - if variable Level in LogMsg(...) call is > than m_LogingLevel none writing to the log file will be done 
	CProcessImageMemory	*m_lpProcessImageMemory;
	CProcessValue m_pvLogingLevel; // for reading reference logging level from PIM - it enables to configure it and change by application
	bool m_bNotInitiated; // must be set to false by calling InitLogger(...) otherwise none logging will be done
	
};



