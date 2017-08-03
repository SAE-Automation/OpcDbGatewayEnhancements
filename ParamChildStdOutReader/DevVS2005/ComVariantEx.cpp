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
** @module ComVariantEx.cpp | 
**
** This source file contains the implementation of the <c Class CComVariantEx>                   
** This class class extends the functionality of the CComVariant class.<nl>
**
************************************************************************************************
** @info
** Version			: 1.0<nl>
** Compiler			: MSVC 6.0<nl>
** Author			: He, SAE - Automation, s.r.o.<nl>
************************************************************************************************
** @history
** He 1.2.2002	Initial version<nl>
** Ko	3.10.03	07.12.2007	EC_0043 - Converts a variant from one type to VT_BSTR, using for VT_R4, VT_R8 the system default LCID.
**
***********************************************************************************************/

#include "stdafx.h"
#include "ComVariantEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComVariantEx::CComVariantEx()
{
}

CComVariantEx::~CComVariantEx()
{
}

CComVariantEx::CComVariantEx( const CComVariantEx& varSrc ):CComVariant(*(CComVariant*)(&varSrc))
{
}

CComVariantEx::CComVariantEx( const CComVariant& varSrc ):CComVariant(varSrc)
{
}

CComVariantEx::CComVariantEx( const VARIANT& varSrc ):CComVariant(varSrc)
{
}

CComVariantEx::CComVariantEx( LPCOLESTR lpsz ):CComVariant(lpsz)
{
}

CComVariantEx::CComVariantEx( LPCSTR lpsz ):CComVariant(lpsz)
{
}

CComVariantEx::CComVariantEx( BSTR bstrSrc ):CComVariant(bstrSrc)
{
}

CComVariantEx::CComVariantEx( bool bSrc ):CComVariant(bSrc)
{
}

CComVariantEx::CComVariantEx( int nSrc ):CComVariant(nSrc)
{
}

CComVariantEx::CComVariantEx( BYTE nSrc ):CComVariant(nSrc)
{
}

CComVariantEx::CComVariantEx( short nSrc ):CComVariant(nSrc)
{
}

CComVariantEx::CComVariantEx( long nSrc, VARTYPE vtSrc):CComVariant(nSrc, vtSrc)
{
}

CComVariantEx::CComVariantEx( float fltSrc ):CComVariant(fltSrc)
{
}

CComVariantEx::CComVariantEx( double dblSrc ):CComVariant(dblSrc)
{
}

CComVariantEx::CComVariantEx( CY cySrc ):CComVariant(cySrc)
{
}

CComVariantEx::CComVariantEx( IDispatch* pSrc ):CComVariant(pSrc)
{
}

CComVariantEx::CComVariantEx( IUnknown* pSrc ):CComVariant(pSrc)
{
}

//*********************************************************************************************************
// @mfunc   CComVariantEx | CComVariantEx | operator+ | Addition
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   The result of the operation
//
// @comm   
//
// @history 1.2.2002 13:34, created by He
//*********************************************************************************************************
CComVariantEx CComVariantEx::operator+( const CComVariantEx& varSrc)
{
	CComVariantEx dest;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		dest = op1.iVal + op2.iVal;
		break;
	case VT_I4:
		dest = op1.lVal + op2.lVal;
		break;
	case VT_R4:
		dest = op1.fltVal + op2.fltVal;
		break;
	case VT_R8:
		dest = op1.dblVal + op2.dblVal;
		break;
	case VT_BSTR:
		{
			_bstr_t bstrDest(op1.bstrVal);
			bstrDest += op2.bstrVal;
			dest = (BSTR)bstrDest; 		
		}
	default:
		break;
	}
	
	return dest;
}
//*********************************************************************************************************
// @mfunc   CComVariantEx | CComVariantEx | operator- | Subtraction
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   The result of the operation
//
// @comm    
//
// @history 1.2.2002 13:34, created by He
//*********************************************************************************************************
CComVariantEx CComVariantEx::operator-( const CComVariantEx& varSrc)
{
	CComVariantEx dest;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		dest = op1.iVal - op2.iVal;
		break;
	case VT_I4:
		dest = op1.lVal - op2.lVal;
		break;
	case VT_R4:
		dest = op1.fltVal - op2.fltVal;
		break;
	case VT_R8:
		dest = op1.dblVal - op2.dblVal;
		break;
	default:
		break;
	}
	
	return dest;
}

//*********************************************************************************************************
// @mfunc   CComVariantEx | CComVariantEx | operator* | Multiplication
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   The result of the operation
//
// @comm    
//
// @history 1.2.2002 13:34, created by He
//*********************************************************************************************************
CComVariantEx CComVariantEx::operator*( const CComVariantEx& varSrc)
{
	CComVariantEx dest;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		dest = op1.iVal * op2.iVal;
		break;
	case VT_I4:
		dest = op1.lVal * op2.lVal;
		break;
	case VT_R4:
		dest = op1.fltVal * op2.fltVal;
		break;
	case VT_R8:
		dest = op1.dblVal * op2.dblVal;
		break;
	default:
		break;
	}
	
	return dest;
}

//*********************************************************************************************************
// @mfunc   CComVariantEx | CComVariantEx | operator+ | Division
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   The result of the operation
//
// @comm    
//
// @history 1.2.2002 13:34, created by He
//*********************************************************************************************************
CComVariantEx CComVariantEx::operator/( const CComVariantEx& varSrc)
{
	CComVariantEx dest;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		if (op2.iVal != 0)
			dest = (DOUBLE)op1.iVal / op2.iVal;
		break;
	case VT_I4:
		if (op2.lVal != 0)
			dest = (DOUBLE)op1.lVal / op2.lVal;
		break;
	case VT_R4:
		if (op2.fltVal != 0.0)
			dest = op1.fltVal / op2.fltVal;
		break;
	case VT_R8:
		if (op2.dblVal != 0.0)
			dest = op1.dblVal / op2.dblVal;
		break;
	default:
		break;
	}
	
	return dest;
}

//*********************************************************************************************************
// @mfunc   BOOL | CComVariantEx | operator> | Is greater
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   Returns one of the following values:
//
// @flag    TRUE  | if the value of this operand is greater than the value of the second operand
// @flag    FALSE | otherwise
//
// @comm    
//
// @history 1.2.2002 13:37, created by He
//*********************************************************************************************************
BOOL CComVariantEx::operator>( const CComVariantEx& varSrc)
{
	BOOL bDest = FALSE;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		if (op1.iVal > op2.iVal)
			bDest = TRUE;
		break;
	case VT_I4:
		if (op1.lVal > op2.lVal)
			bDest = TRUE;
		break;
	case VT_R4:
		if (op1.fltVal > op2.fltVal)
			bDest = TRUE;
		break;
	case VT_R8:
		if (op1.dblVal > op2.dblVal)
			bDest = TRUE;
		break;
	case VT_BSTR:
		{
			_bstr_t bstrOp1(op1.bstrVal);
			_bstr_t bstrOp2(op2.bstrVal);
			if (bstrOp1 > bstrOp2)
				bDest = TRUE;
		}
	default:
		break;
	}
	
	return bDest;
}

//*********************************************************************************************************
// @mfunc   BOOL | CComVariantEx | operator> | Is lower
//
// @parm    const | CComVariantEx& varSrc | [in] The second operand
//
// @rdesc   Returns one of the following values:
//
// @flag    TRUE  | if the value of this operand is lower than the value of the second operand
// @flag    FALSE | otherwise
//
// @comm    
//
// @history 1.2.2002 13:37, created by He
//*********************************************************************************************************
BOOL CComVariantEx::operator<( const CComVariantEx& varSrc)
{
	BOOL bDest = FALSE;
	CComVariantEx op1(*this);
	CComVariantEx op2(varSrc);

	VARTYPE vt = FindCommonVariantType(this->vt, varSrc.vt);
	
	op1.ChangeType(vt);
	op2.ChangeType(vt);
	
	switch(vt)
	{
	case VT_I2:	
		if (op1.iVal < op2.iVal)
			bDest = TRUE;
		break;
	case VT_I4:
		if (op1.lVal < op2.lVal)
			bDest = TRUE;
		break;
	case VT_R4:
		if (op1.fltVal < op2.fltVal)
			bDest = TRUE;
		break;
	case VT_R8:
		if (op1.dblVal < op2.dblVal)
			bDest = TRUE;
		break;
	case VT_BSTR:
		{
			_bstr_t bstrOp1(op1.bstrVal);
			_bstr_t bstrOp2(op2.bstrVal);
			if (bstrOp1 < bstrOp2)
				bDest = TRUE;
		}
	default:
		break;
	}
	
	return bDest;
}

//*********************************************************************************************************
// @mfunc   VARTYPE | CComVariantEx | FindCommonVariantType | Find the nearest common variant type of two variant types
//
// @parm    VARTYPE | vt1 | [in] the first variant type
//
// @parm    VARTYPE | vt2 | [in] the second variant type
//
// @rdesc   Returns one of the following variant types VT_EMPTY, VT_I2, VT_I4, VT_R4, VT_R8 or VT_BSTR.
//
// @comm    VT_EMPTY is returned if there is no convenient variant type found.
//
// @history 1.2.2002 14:18, created by He
//*********************************************************************************************************
VARTYPE CComVariantEx::FindCommonVariantType(VARTYPE vt1, VARTYPE vt2)
{
	VARTYPE vt = VT_EMPTY;
	VARTYPE vtOp1 = FindBestVariantType(vt1);
	VARTYPE vtOp2 = FindBestVariantType(vt2);

	switch (vtOp1)
	{
	case VT_I2:
		if (vtOp2 == VT_I4 || vtOp2 == VT_R4 || vtOp2 == VT_R8 || vtOp2 == VT_BSTR)
			vt = vtOp2;
		else if (vtOp2 == VT_I2)
			vt = vtOp1;
		break;
	case VT_I4:
		if (vtOp2 == VT_R4 || vtOp2 == VT_R8 || vtOp2 == VT_BSTR)
			vt = vtOp2;
		else if (vtOp2 == VT_I4 || vtOp2 == VT_I2)
			vt = vtOp1;
		break;
	case VT_R4:
		if (vtOp2 == VT_R8 || vtOp2 == VT_BSTR)
			vt = vtOp2;
		else if (vtOp2 == VT_R4 || vtOp2 == VT_I4 || vtOp2 == VT_I2)
			vt = vtOp1;
		break;
	case VT_R8:
		if (vtOp2 == VT_BSTR)
			vt = vtOp2;
		else if (vtOp2 == VT_R8 || vtOp2 == VT_R4 || vtOp2 == VT_I4 || vtOp2 == VT_I2)
			vt = vtOp1;
		break;
	case VT_BSTR:
		if (vtOp2 != VT_EMPTY)
			vt = vtOp1;
		break;
	default:
		break;
	}
	return vt;
}

//*********************************************************************************************************
// @mfunc   VARTYPE | CComVariantEx | FindBestVariantType | Find the best alternative variant type for 
//                                                          arithemtic operation
//
// @parm    VARTYPE | vt | [in] the variant type 
//
// @rdesc   Returns one of the following types: VT_I2, VT_I4, VT_R4, VT_R8, VT_BSTR or VT_EMPTY
//
//
// @comm    Not all variant types are fully suported by Microsoft. Some of the types e.g VT_UIx can't be used
//          in the method ChangeType. Therefore it is better to find the best alternative for these types.
//
// @history 1.2.2002 14:23, created by He
//*********************************************************************************************************
VARTYPE CComVariantEx::FindBestVariantType(VARTYPE vt)
{
	switch (vt)
	{
	case VT_I1:
	case VT_UI1:
	case VT_BOOL:
		vt = VT_I2;
		break;

	case VT_I2:
	case VT_I4:
	case VT_R4:
	case VT_R8:
	case VT_BSTR:
		break;

	case VT_UI2:
		vt = VT_I4;
		break;

	case VT_UI4:
		vt= VT_R8;
		break;

	default:
		vt = VT_EMPTY;
		break;
	}

	return vt;
}

//*********************************************************************************************************
// @mfunc   HRESULT | CComVariantEx | ChangeTypeToBSTR | Converts a variant from one type to VT_BSTR, using for VT_R4, VT_R8 the system default LCID.
//
// @rdesc   Returns one of the following values:
//
// @flag    S_OK      | Successfully
// @flag    Otherwise | Error
//
// @comm    Converts a variant from one type to VT_BSTR, using the system default LCID.
//
// @history 07.12.2007 11:13, created by Ko
//		Ko - EC_0043 - Converts a variant from one type to VT_BSTR, using for VT_R4, VT_R8 the system default LCID.
//*********************************************************************************************************
HRESULT CComVariantEx::ChangeTypeToBSTR()
{
	switch (vt)
	{
		case VT_R4:
		case VT_R8:			
			return VariantChangeTypeEx(this, this, GetSystemDefaultLCID(), 0, VT_BSTR);

		default:
			break;
	}

	return ChangeType(VT_BSTR);
}
