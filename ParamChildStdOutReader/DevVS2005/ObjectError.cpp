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
** @module ObjectError.cpp | Handles an occurred error
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

#include "stdafx.h"
#include "ObjectError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectError::CObjectError()
{
	m_bstrLastError="";
}

CObjectError::~CObjectError()
{

}

//*********************************************************************************************************
// @mfunc   _bstr_t | CObjectError | GetLastError | Gets the last-error description for the error
//
// @rdesc   Returns one of the following values:
//
// @flag    Gets the last-error description for the error
//
// @comm    
//
// @history 30.1.2002 07:41, created by Ko
//*********************************************************************************************************
_bstr_t CObjectError::GetLastError()
{

	return m_bstrLastError;
}

//*********************************************************************************************************
// @mfunc   void | CObjectError | SetLastError | Sets the last-error description for the error
//
// @parm    _com_error | objError | A Error object
//
// @rdesc   Returns one of the following values:
//
// @flag    void
//
// @comm    
//
// @history 30.1.2002 07:44, created by Ko
//*********************************************************************************************************
void CObjectError::SetLastError(_com_error objError)
{

	_bstr_t bstrLastError;

	bstrLastError= "Error code : " + _bstr_t(objError.Error()) + "\n";
	bstrLastError = bstrLastError + "Source       : " + objError.Source() + "\n";
	bstrLastError = bstrLastError + "Description  : " + objError.Description() + "\n";
	bstrLastError = bstrLastError + "Error Message: " + objError.ErrorMessage();

	// @flow0 | Sets an error description
	m_bstrLastError = bstrLastError;

	return;
}

//*********************************************************************************************************
// @mfunc   void | CObjectError | SetLastError | Sets the last-error description for the error
//
// @parm    _bstr_t | bstrError | An error description
//
// @rdesc   Returns one of the following values:
//
// @flag    void
//
// @comm    
//
// @history 30.1.2002 07:46, created by Ko
//*********************************************************************************************************
void CObjectError::SetLastError(_bstr_t bstrError)
{

	_bstr_t bstrLastError;

	bstrLastError= "Error code: Internal\n";
	bstrLastError = bstrLastError + "Source     : Internal\n";
	bstrLastError = bstrLastError + "Description: " + bstrError;

	// @flow0 | Sets an error description
	m_bstrLastError = bstrLastError;

	return;
}

