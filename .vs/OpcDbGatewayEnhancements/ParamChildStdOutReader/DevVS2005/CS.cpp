/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o., Nova Dubnica, Slovakia,  					   *
**																							   *
************************************************************************************************
**																							   *
** Project: OpcDbGateway		Project name:  NeCST	                                       *
**																							   *
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 7.0<nl>
** Author			: Jozef Koys, SAE - Automation, s.r.o., <nl>
************************************************************************************************
** @history
** Ko	3.12.00	14.09.2009	EC_0060 - Migration to version ICONICS OPC ToolWorX 3.50.
**                                  - In this version was created this CS class because ...
**                                    ICONICS CCriticalSection class was removed from ToolWorX. 
***********************************************************************************************/
#include "StdAfx.h"
#include ".\cs.h"

CCS::CCS(void)
{
	//m_bInitOK = true;
	__try
	{
		::InitializeCriticalSection(&m_CS);
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//m_bInitOK = false;
	}
}

CCS::~CCS(void)
{
	//if (m_bInitOK)
		::DeleteCriticalSection(&m_CS);
}

void CCS::Enter()
{
	//if (m_bInitOK)
		::EnterCriticalSection(&m_CS);
}

void CCS::Leave()
{
	//if (m_bInitOK)
		::LeaveCriticalSection(&m_CS);
}