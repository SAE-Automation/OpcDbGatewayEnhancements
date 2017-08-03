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
** @module ProcessImageMemory.cpp | 
**
** This source file contains the implementation of the <c Class CProcessImageMemory>                   
** These classes implements .... .<nl>
**
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 6.0<nl>
** Author			: He, SAE - Automation, s.r.o.<nl>
************************************************************************************************
** @history
** He 24.1.2002	Initial version<nl>
**
***********************************************************************************************/

#include "stdafx.h"
#include "ProcessImageMemory.h"
#include <bitset>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CProcessImageMemory processImageMemory_g;

CProcessImageMemory::CProcessImageMemory()
{
	m_dwMaxAddress = 0;
	m_pProcMem = NULL;
}

CProcessImageMemory::~CProcessImageMemory()
{
	Clear();
}

void CProcessImageMemory::AddUpdateHandler(ProcessMOUpdateHandler_t pHandler)
{
	m_cUpdateHandlerVector.push_back(pHandler);
}

void CProcessImageMemory::RemoveUpdateHandler(ProcessMOUpdateHandler_t pHandler)
{
	for (INT_PTR i = m_cUpdateHandlerVector.size() - 1; i >=0; i--)
	{
		if (m_cUpdateHandlerVector[i] == pHandler)
			m_cUpdateHandlerVector.erase(m_cUpdateHandlerVector.begin( ) + i);
	}
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Init | Init the process image memory
//
// @parm    DWORD | dwMaxAddress | [in] The maximum address of the memory
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
//
// @comm    
//
// @history 24.1.2002 11:05, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Init(const FILETIME &ActualTime, DWORD dwMaxAddress)
{
	m_dwMaxAddress = dwMaxAddress;
	if (m_dwMaxAddress >= MEMORY_SIZE)
		m_dwMaxAddress = MEMORY_SIZE-1;
	else if (m_dwMaxAddress <= SYSTEM_SIZE)
		m_dwMaxAddress = SYSTEM_SIZE-1;

	m_pProcMem = new CProcessValue*[m_dwMaxAddress + 1];
	m_pProcMem[0] = NULL;
	for (DWORD i = 1 ; i <= m_dwMaxAddress ; i++)		// TS 19.10. 2012	EC_00XX TODO
	{
		m_pProcMem[i] = new CProcessValue;
		m_pProcMem[i]->vValue.Clear();
		m_pProcMem[i]->wQuality = OPC_QUALITY_GOOD;
		m_pProcMem[i]->ftTimeStamp = ActualTime;
		m_pProcMem[i]->dwID = 0;
		m_pProcMem[i]->wTimeStampType = CProcessValue::TimeStampInternal;
	}
	return S_OK;
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Clear | Clear process image memory, release allocated memory
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
//
// @comm    
//
// @history 24.1.2002 11:06, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Clear()
{
	m_cs.Enter();

	for (DWORD i=1 ; i <= m_dwMaxAddress; i++)		// TS 19.10. 2012	EC_00XX TODO
	{
		if (m_pProcMem[i]!=NULL)
			delete m_pProcMem[i];
		
		m_pProcMem[i]=NULL;
	}

	m_dwMaxAddress = 0;

	if(m_pProcMem != NULL)
	{
		delete []m_pProcMem;
		
		m_pProcMem = NULL;
	}

	m_cs.Leave();

	return S_OK;
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Read | Read a value from the process image memory
//
// @parm    DWORD | VarAddress | [in] The address of the value
//
// @parm    CProcessValue | *pValue | [out] The place to store the value from the memory
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
// @flag    E_FAIL | Operation failed. Call GetLastError for error description.
//
// @comm    
//
// @history 24.1.2002 11:07, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Read(DWORD VarAddress, CProcessValue *pValue)
{
	_ASSERT(pValue != NULL);
	HRESULT hr = S_OK;

	if (pValue == NULL)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" Invalid argument pValue");
		hr = E_POINTER;
		SetLastError (bstrErr);
		return hr;
	}

	if (VarAddress > m_dwMaxAddress)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" The address is out of range!");
		hr = E_FAIL;
		SetLastError (bstrErr);
		return hr;
	}

	if (m_pProcMem == NULL || m_pProcMem[VarAddress] == NULL)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" VarAddress doesn't exist in the memory!");
		hr = E_FAIL;
		SetLastError(bstrErr);
		return hr;
	}

	m_cs.Enter();

	if (m_pProcMem != NULL && m_pProcMem[VarAddress] != NULL)
	{
		pValue->dwID = m_pProcMem[VarAddress]->dwID;
		pValue->ftTimeStamp = m_pProcMem[VarAddress]->ftTimeStamp;
		pValue->wQuality = m_pProcMem[VarAddress]->wQuality;
		pValue->vValue = m_pProcMem[VarAddress]->vValue;
		pValue->wTimeStampType = m_pProcMem[VarAddress]->wTimeStampType;
	}
	else
		hr = E_FAIL;
	
	m_cs.Leave();

	return hr;
}

HRESULT CProcessImageMemory::WriteInternal(DWORD VarAddress, CProcessValue &Value)
{
	HRESULT hr = S_OK;

	if (VarAddress > m_dwMaxAddress)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Write() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" The address is out of range!");
		hr = E_FAIL;
		SetLastError (bstrErr);
		return hr;
	}
	
	if (m_pProcMem != NULL && m_pProcMem[VarAddress] != NULL)
	{
		_ASSERT(!((Value.dwID != 0) && (m_pProcMem[VarAddress]->dwID != 0) && (Value.dwID != m_pProcMem[VarAddress]->dwID)));
		if (Value.dwID != 0)
			m_pProcMem[VarAddress]->dwID = Value.dwID;
		m_pProcMem[VarAddress]->ftTimeStamp = Value.ftTimeStamp;
		m_pProcMem[VarAddress]->wQuality = Value.wQuality;
		m_pProcMem[VarAddress]->vValue = Value.vValue;

		if (Value.wTimeStampType == CProcessValue::TimeStampInternal || 
			Value.wTimeStampType == CProcessValue::TimeStampExternal)
		m_pProcMem[VarAddress]->wTimeStampType = Value.wTimeStampType;
		else
			m_pProcMem[VarAddress]->wTimeStampType = CProcessValue::TimeStampInternal;
	}
	else
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Write() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" VarAddress doesn't exist in the memory!");
		SetLastError(bstrErr);
		hr = E_FAIL;
	}

	return hr;
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Write | Write a value to the process image memory
//
// @parm    DWORD | VarAddress | [in] The address in the process image memory
//
// @parm    CProcessValue | &Value | [in] The new value 
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
// @flag    E_FAIL | Operation failed. Call GetLastError to get the error description
//
// @comm    
//
// @history 24.1.2002 11:08, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Write(DWORD VarAddress, CProcessValue &Value)
{
	HRESULT hr = S_OK;

	m_cs.Enter();

	hr = WriteInternal(VarAddress, Value);
		
	m_cs.Leave();

	if (hr == S_OK && m_cUpdateHandlerVector.size())
	{
		for (size_t i = 0; i < m_cUpdateHandlerVector.size(); i++)
		{
			m_cUpdateHandlerVector[i](VarAddress, Value);
		}
	}

	return hr;
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | GetMaxAddress | Get the maximum address of the process image memory 
//
// @rdesc   Returns the maximum address
//
// @comm    
//
// @history 24.1.2002 11:10, created by He
//*********************************************************************************************************
DWORD CProcessImageMemory::GetMaxAddress()
{
	return m_dwMaxAddress;
}

//*********************************************************************************************************
// @mfunc   HRESULT | CProcessImageMemory | Write | Write a value to the proces simage memory
//
// @parm    DWORD | dwAddress | [in] The address of the process image memory
//
// @parm    const CComVariant& | vValue | [in] The new value
//
// @parm    WORD | wQuality | [in] The quality of the value
//	
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
// @flag    E_FAIL | Operation failed.
//
// @comm    
//
// @history 24.1.2002 11:10, created by He
//*********************************************************************************************************

HRESULT CProcessImageMemory::Write(DWORD dwAddress, const CComVariant& vValue, WORD wQuality/*=OPC_QUALITY_GOOD*/)
{
	CProcessValue procValue;
	procValue.dwID = 0;
	procValue.wQuality = wQuality;
	procValue.wTimeStampType = CProcessValue::TimeStampInternal;
	procValue.vValue = vValue;
	CoFileTimeNow(&procValue.ftTimeStamp);

	return Write(dwAddress, procValue);
}


HRESULT CProcessImageMemory::Write(DWORD dwAddress, const CComVariant& vValue, const FILETIME &ftTimeStamp, WORD wQuality)
{
	CProcessValue procValue;
	procValue.dwID = 0;
	procValue.wQuality = wQuality;
	procValue.wTimeStampType = CProcessValue::TimeStampInternal;
	procValue.vValue = vValue;
	procValue.ftTimeStamp = ftTimeStamp;

	return Write(dwAddress, procValue);
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Read | Read a value from the process image memory
//
// @parm    DWORD | VarAddress	| [in] The address of the value
//
// @parm    DWORD | BitNr		| [in] The number of the bit to write to
//
// @parm    CProcessValue | *pValue | [out] The place to store the value from the memory
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
// @flag    E_FAIL | Operation failed. Call GetLastError for error description.
//
// @comm    
//
// @history 24.1.2002 11:07, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Read(DWORD VarAddress, DWORD BitNr, CProcessValue *pValue)
{
	_ASSERT(pValue != NULL);
	HRESULT hr = S_OK;

	if (pValue == NULL)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" Invalid argument pValue");
		hr = E_POINTER;
		SetLastError (bstrErr);
		return hr;
	}

	if (VarAddress > m_dwMaxAddress)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" The address is out of range!");
		hr = E_FAIL;
		SetLastError (bstrErr);
		return hr;
	}

	if (m_pProcMem == NULL || m_pProcMem[VarAddress] == NULL)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Read() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" VarAddress doesn't exist in the memory!");
		hr = E_FAIL;
		SetLastError(bstrErr);
		return hr;
	}

	m_cs.Enter();

	if (m_pProcMem != NULL && m_pProcMem[VarAddress] != NULL)
	{
		pValue->dwID = m_pProcMem[VarAddress]->dwID;
		pValue->ftTimeStamp = m_pProcMem[VarAddress]->ftTimeStamp;
		pValue->wQuality = m_pProcMem[VarAddress]->wQuality;

		// Only the first four bytes of the variant are evaluated
		std::bitset<32> lBitSet(m_pProcMem[VarAddress]->vValue.intVal); 
		pValue->vValue = lBitSet[BitNr];
		
		pValue->wTimeStampType = m_pProcMem[VarAddress]->wTimeStampType;
	}
	else
		hr = E_FAIL;
	
	m_cs.Leave();

	return hr;
}

//*********************************************************************************************************
// @mfunc   DWORD | CProcessImageMemory | Write | Write a value to the process image memory
//
// @parm    DWORD | VarAddress | [in] The address in the process image memory
//
// @parm    DWORD | BitNr	   | [in] The number of the bit where the value should be written to
//
// @parm    CProcessValue	   | &Value | [in] The new value 
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK   | Success
// @flag    E_FAIL | Operation failed. Call GetLastError to get the error description
//
// @comm    
//
// @history 24.1.2002 11:08, created by He
//*********************************************************************************************************
HRESULT CProcessImageMemory::Write(DWORD VarAddress, DWORD BitNr, CProcessValue &Value)
{
	HRESULT hr = S_OK;

	CProcessValue cValue;

	if (VarAddress > m_dwMaxAddress)
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Write() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" The address is out of range!");
		hr = E_FAIL;
		SetLastError(bstrErr);
		return hr;
	}

	m_cs.Enter();

	if (m_pProcMem != NULL && m_pProcMem[VarAddress] != NULL)
	{
		_ASSERT(!((Value.dwID != 0) && (m_pProcMem[VarAddress]->dwID != 0) && (Value.dwID != m_pProcMem[VarAddress]->dwID)));
		if (Value.dwID != 0)
			cValue.dwID = Value.dwID;
		else
			cValue.dwID = 0;
		cValue.ftTimeStamp = Value.ftTimeStamp;
		cValue.wQuality = Value.wQuality;
		std::bitset<32> lBitSet(m_pProcMem[VarAddress]->vValue.intVal);
		lBitSet.set(BitNr, (Value.vValue.boolVal & 0x1));
		cValue.vValue = (LONG)(lBitSet.to_ulong());

		if (Value.wTimeStampType == CProcessValue::TimeStampInternal || 
			Value.wTimeStampType == CProcessValue::TimeStampExternal)
		cValue.wTimeStampType = Value.wTimeStampType;
		else
			cValue.wTimeStampType = CProcessValue::TimeStampInternal;

		*m_pProcMem[VarAddress] = cValue;
	}
	else
	{
		_bstr_t bstrErr = _T("CProcessImageMemory::Write() VarAddress=") + _bstr_t((LONG)VarAddress);
		bstrErr += _T(" VarAddress doesn't exist in the memory!");
		SetLastError(bstrErr);
		hr = E_FAIL;
	}
		
	m_cs.Leave();

	if (hr == S_OK && m_cUpdateHandlerVector.size())
	{
		for (size_t i = 0; i < m_cUpdateHandlerVector.size(); i++)
		{
			m_cUpdateHandlerVector[i](VarAddress, cValue);
		}
	}

	return hr;
}

HRESULT CProcessImageMemory::Write(std::map<DWORD, CProcessValue> &cValueMap)
{
	HRESULT hr = S_OK, hr2 = S_OK;
	std::set<DWORD> cFailedWriteSet;

	if (cValueMap.empty())
		return S_FALSE;

	m_cs.Enter();

	std::map<DWORD, CProcessValue>::iterator IPV = cValueMap.begin();
	while (IPV != cValueMap.end())
	{
		hr2 = WriteInternal(IPV->first, IPV->second);
		if (FAILED(hr2))
		{
			hr = S_FALSE;
			cFailedWriteSet.insert(IPV->first);
		}
		IPV++;
	}
		
	m_cs.Leave();

	if (SUCCEEDED(hr) && m_cUpdateHandlerVector.size())
	{
		for (size_t i = 0; i < m_cUpdateHandlerVector.size(); i++)
		{
			IPV = cValueMap.begin();
			while (IPV != cValueMap.end())
			{
				if (cFailedWriteSet.find(IPV->first) == cFailedWriteSet.end())
				{
					m_cUpdateHandlerVector[i](IPV->first, IPV->second);
				}
				IPV++;
			}
		}
	}

	return hr;
}
