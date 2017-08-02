/***********************************************************************************************
**																							   *
** Copyright: 				SAE - Automation, s.r.o., Nova Dubnica, Slovakia,				   *
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

#pragma once
#pragma pack(push)
#pragma pack(8)

class CCS
{
public:
	CCS(void);
	~CCS(void);

	void Enter();
	void Leave();

private:
	CRITICAL_SECTION	m_CS;
	//bool				m_bInitOK;
};

#pragma pack(pop)
