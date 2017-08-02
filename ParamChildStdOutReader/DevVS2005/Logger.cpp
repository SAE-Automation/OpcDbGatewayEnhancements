#include "stdafx.h"
#include "Logger.h"

#define IS_VALID_MO_ADDRESS(address) ((address != (DWORD)-1) && (address != 0))
#define IS_VALID_USER_MO_ADDRESS(address) (IS_VALID_MO_ADDRESS(address) && (address > 100))


CLogger::CLogger()
	: m_lpProcessImageMemory(NULL), 
	m_LogingLevel(LOGING_LEVEL_INFORMATION), 
	m_bNotInitiated(true) // it must be true till InitLogger(...) will be called
{
}
CLogger::~CLogger(void)
{
}

bool CLogger::InitLogger(__in CProcessImageMemory *pProcessImageMemory,
						 __in  LOGING_LEVEL LogingLevel, // logging level from that this logging method will be fullfilled
						 __in DWORD dwLogParameterAddrValue
						 )
{
	if(pProcessImageMemory == NULL)
		return false;				// surely can not be initialised
	
	m_lpProcessImageMemory = pProcessImageMemory;
	m_LogingLevel = LOGING_LEVEL_ALL; //default logging level
	m_bNotInitiated = false; // initialisation of logger is done - InitLogger(...) has been called

	if ( ! (IS_VALID_USER_MO_ADDRESS( dwLogParameterAddrValue )) )//If not defined or not correct Login Level will not be read from PIM but taken from LogingLevel input parameter 
	{
		m_LogingLevel = LogingLevel; // Login level is not set from PIM
		return true;
	}
	else
	{
		if ( ! ReadConfiguredLoggingLevel(dwLogParameterAddrValue) )
		{
	
			return true; // default logging level will be used 
		}
		else
		{
			m_LogingLevel = (LOGING_LEVEL)(m_pvLogingLevel.vValue.intVal);//TODO: preveri+t ci to bude fungova+t s t˝mto typom 
			return true;
		}
	}
		

}

bool CLogger::ReadConfiguredLoggingLevel(__in DWORD dwLogParameterAddrValue)
{
	if ( ! (IS_VALID_USER_MO_ADDRESS( dwLogParameterAddrValue )) )
		return false;
	if ( IS_VALID_USER_MO_ADDRESS (dwLogParameterAddrValue) && 
		(S_OK == m_lpProcessImageMemory->Read(dwLogParameterAddrValue, &m_pvLogingLevel)))
		return true;
	else
		return false;
}

void CLogger::LogMsg(__in LOGING_LEVEL Level,// it  must be <= m_LogingLevel to provide writing to the log file
	__in const CString &szMsg)
{
   if (m_lpProcessImageMemory == NULL || Level > m_LogingLevel || m_bNotInitiated )
      return;

   CString sLogMsg;
   CComVariant cVariantNewMsg;
   CProcessValue cPIMMsgTrigger, cPIMMsgStr;

   if (S_OK != m_lpProcessImageMemory->Read(CProcessImageMemory::SYSADDR_LOG_MSG_TRIGGER, &cPIMMsgTrigger))
      return;
   if (cPIMMsgTrigger.vValue.vt != VT_EMPTY && cPIMMsgTrigger.vValue.vt != VT_BOOL)
      return;

   if (S_OK != m_lpProcessImageMemory->Read(CProcessImageMemory::SYSADDR_LOG_MSG_STR, &cPIMMsgStr))
      return;
   if (cPIMMsgStr.vValue.vt != VT_EMPTY && cPIMMsgStr.vValue.vt != VT_BSTR)
      return;

   sLogMsg = (TCHAR*)_bstr_t(cPIMMsgStr.vValue.bstrVal);

   if (sLogMsg.IsEmpty())
      sLogMsg = szMsg;
   else
   {
      if (sLogMsg.GetLength() >= 1023)
         return; // Uz neloguj. Nema to vyznam. Pockaj na uvolnenie.
      sLogMsg += _T("\t");
      sLogMsg += szMsg;
   }

   if (sLogMsg.GetLength() >= 1023)
      sLogMsg.Left(1023);
   
   cVariantNewMsg.ChangeType(VT_BSTR);
   cVariantNewMsg = (LPCSTR)sLogMsg;

   m_lpProcessImageMemory->Write(CProcessImageMemory::SYSADDR_LOG_MSG_STR, cVariantNewMsg);
   
   cPIMMsgTrigger.vValue.ChangeType(VT_BOOL);
   cPIMMsgTrigger.vValue.boolVal = VARIANT_TRUE;
   m_lpProcessImageMemory->Write(CProcessImageMemory::SYSADDR_LOG_MSG_TRIGGER, cPIMMsgTrigger.vValue);
}

void CLogger::LogMsg(__in LOGING_LEVEL Level, __in UINT nMsgID)
{
   if (m_lpProcessImageMemory == NULL || Level > m_LogingLevel || m_bNotInitiated)
      return;
   CString cszErrorMsg;
   cszErrorMsg.LoadString(nMsgID);
   LogMsg(Level, cszErrorMsg);
}

void CLogger::LogMsg1(__in LOGING_LEVEL Level, __in UINT nMsgID, __in DWORD dwValue)
{
   if (m_lpProcessImageMemory == NULL || Level > m_LogingLevel|| m_bNotInitiated)
      return;
   CString cszErrorMsg;
   cszErrorMsg.Format(nMsgID, dwValue);
   LogMsg(Level, cszErrorMsg);
}

void CLogger::LogMsg1(__in LOGING_LEVEL Level, __in UINT nMsgID, __in const CString &szValue)
{
   if (m_lpProcessImageMemory == NULL || Level > m_LogingLevel || m_bNotInitiated)
      return;

   CString cszErrorMsg;
   cszErrorMsg.Format(nMsgID, szValue);
   LogMsg(Level, cszErrorMsg);
}

HRESULT CLogger::ClearLogMsg(void)
{
	CComVariant cVariantEmptyMsg;
	cVariantEmptyMsg.ChangeType(VT_BSTR);
	cVariantEmptyMsg.bstrVal = _bstr_t(_T(""));
	return m_lpProcessImageMemory->Write(CProcessImageMemory::SYSADDR_LOG_MSG_STR, cVariantEmptyMsg, OPC_QUALITY_GOOD); // Vycistime log
}
bool CLogger::CanWriteToLog(void)
{
	_ASSERT(m_lpProcessImageMemory != NULL);

	//Test pripravenosti z·pisu do log s˙boru
	CProcessValue vState;
	while(1){ // cakanie v cykle na vynulovanie priznaku zapisu do logu
		if(SUCCEEDED(m_lpProcessImageMemory->Read(CProcessImageMemory::SYSADDR_LOG_MSG_TRIGGER, &vState)))
		{
			if(vState.vValue.vt==VT_BOOL && !vState.vValue.boolVal) 
				return true; //ak prÌznak z·pisu do logu je vynulovan˝
		}
		else
			return false;	// ak sa ned· ËÌtaù z MO - do logu by sme aj tak nevedeli zapisat	
		
		Sleep(100); // ak sa ned· ËÌtaù z MO - do logu by sme aj tak nevedeli zapisat
	}
	return false;
}

