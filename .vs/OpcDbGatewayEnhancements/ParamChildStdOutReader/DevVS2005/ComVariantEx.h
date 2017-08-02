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
** @module ComVariantEx.h | 
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

#if !defined(AFX_COMVARIANTEX_H__78656317_D781_417A_8505_283F08C93449__INCLUDED_)
#define AFX_COMVARIANTEX_H__78656317_D781_417A_8505_283F08C93449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComVariantEx : public CComVariant  
{
public:
	CComVariantEx();
	virtual ~CComVariantEx();

	CComVariantEx( const CComVariantEx& varSrc );
	CComVariantEx( const CComVariant& varSrc );
	CComVariantEx( const VARIANT& varSrc );
	CComVariantEx( LPCOLESTR lpsz );
	CComVariantEx( LPCSTR lpsz );
	CComVariantEx( BSTR bstrSrc );
	CComVariantEx( bool bSrc );
	CComVariantEx( int nSrc );
	CComVariantEx( BYTE nSrc );
	CComVariantEx( short nSrc );
	CComVariantEx( long nSrc, VARTYPE vtSrc = VT_I4 );
	CComVariantEx( float fltSrc );
	CComVariantEx( double dblSrc );
	CComVariantEx( CY cySrc );
	CComVariantEx( IDispatch* pSrc );
	CComVariantEx( IUnknown* pSrc );

public:
	CComVariantEx& operator =( const CComVariantEx& varSrc );
	CComVariantEx& operator =( const VARIANT& varSrc );
	CComVariantEx& operator =( LPCOLESTR lpsz );
	CComVariantEx& operator =( LPCSTR lpsz );
	CComVariantEx& operator =( BSTR bstrSrc );
	CComVariantEx& operator =( bool bSrc );
	CComVariantEx& operator =( int nSrc );
	CComVariantEx& operator =( BYTE nSrc );
	CComVariantEx& operator =( short nSrc );
	CComVariantEx& operator =( long nSrc );
	CComVariantEx& operator =( float nSrc );
	CComVariantEx& operator =( double nSrc );
	CComVariantEx& operator =( CY cySrc );
	CComVariantEx& operator =( IDispatch* pSrc );
	CComVariantEx& operator =( IUnknown* pSrc );

	CComVariantEx operator+( const CComVariantEx& varSrc);
	CComVariantEx operator-( const CComVariantEx& varSrc);
	CComVariantEx operator*( const CComVariantEx& varSrc);
	CComVariantEx operator/( const CComVariantEx& varSrc);
	BOOL operator>( const CComVariantEx& varSrc);
	BOOL operator<( const CComVariantEx& varSrc);

	HRESULT ChangeTypeToBSTR();								// Ko - EC_0043
protected:
	VARTYPE FindCommonVariantType(VARTYPE vt1, VARTYPE vt2);
	VARTYPE FindBestVariantType(VARTYPE vt);
};

inline CComVariantEx& CComVariantEx::operator =( const CComVariantEx& varSrc )
{
	(*(CComVariant*)this) = (*(CComVariant*)&varSrc); 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( const VARIANT& varSrc )
{
	(*(CComVariant*)this) = varSrc; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( LPCOLESTR lpsz )
{ 
	(*(CComVariant*)this) = lpsz; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( LPCSTR lpsz )
{
	(*(CComVariant*)this) = lpsz; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( BSTR bstrSrc )
{ 
	(*(CComVariant*)this) = bstrSrc; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( bool bSrc )
{
	(*(CComVariant*)this) = bSrc; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( int nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( BYTE nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( short nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( long nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline CComVariantEx& CComVariantEx::operator =( float nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( double nSrc )
{
	(*(CComVariant*)this) = nSrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( CY cySrc )
{
	(*(CComVariant*)this) = cySrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( IDispatch* pSrc )
{
	(*(CComVariant*)this) = pSrc; 
	return *this;
}

inline	CComVariantEx& CComVariantEx::operator =( IUnknown* pSrc )
{
	(*(CComVariant*)this) = pSrc; 
	return *this;
}

#endif // !defined(AFX_COMVARIANTEX_H__78656317_D781_417A_8505_283F08C93449__INCLUDED_)
