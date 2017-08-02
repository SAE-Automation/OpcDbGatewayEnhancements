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
** @module ObjectError.h | Handles an occurred error
**
** This source file contains the implementation of the <c CObjectError>                   
** This class handles an occurred error<nl>
**
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 6.0<nl>
** Author			: Ko, SAE - Automation, s.r.o., <nl>
************************************************************************************************
** @history
** Ko	28.02.2000	Initial version<nl>
**
***********************************************************************************************/

#pragma once
#pragma pack(push)
#pragma pack(8)

//@class  This class handles an occurred error
class CObjectError  
{
// CONSTRUCTOR + DESTRUCTOR
public:
	// @cmember:Constructor
	CObjectError();
	// @cmember:Constructor
	virtual ~CObjectError();


// METHOD
public:
	// @cmember:Gets the last-error description for the error
	_bstr_t GetLastError();

protected:
	// @cmember:Sets the last-error description for the error
	virtual void SetLastError(_com_error objError);
	// @cmember:Sets the last-error description for the error
	virtual void SetLastError(_bstr_t bstrError);


// MEMBER
protected:
	// @cmember:An error description
	_bstr_t m_bstrLastError; 
};

#pragma pack(pop)