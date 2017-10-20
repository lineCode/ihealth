#include "StdAfx.h"
#include "dcidata.h"

using namespace Ext::Ado;

CDCIData::CDCIData(void)
{
}

CDCIData::~CDCIData(void)
{
}

CDCIData::CDCIData(bool bSrc)
{
	*this = bSrc;
}

CDCIData::CDCIData( BYTE bSrc )
{
	*this = bSrc;
}

CDCIData::CDCIData( short sSrc )
{
	*this = sSrc;
}

CDCIData::CDCIData( long lSrc )
{
	*this = lSrc;
}

CDCIData::CDCIData( float fltSrc )
{
	*this = fltSrc;
}

CDCIData::CDCIData( double dblSrc )
{
	*this = dblSrc;
}

CDCIData::CDCIData( LPCTSTR pSrc )
{
	*this = pSrc;
}

CDCIData::CDCIData( const _variant_t& vVal )
{
	*this = vVal;
}


CDCIData::CDCIData( const CDCIData& vVal )
{
	*this = vVal;
}

CDCIData::CDCIData( int nVal )
{
	*this = bVal;
}

//CDCIData::CDCIData( const CString& strVal )
//{
//	*this = strVal;
//}

CDCIData::CDCIData( const COleDateTime& tVal )
{
	*this = tVal;
}

CDCIData::CDCIData( const CTime& tVal )
{
	*this = tVal;
}

CDCIData& CDCIData::operator=( BYTE bSrc )
{
	this->vt = VT_UI1;
	this->bVal = bSrc;

	return *this;
}

CDCIData& CDCIData::operator=( bool bSrc )
{
	this->vt = VT_BOOL;
	this->boolVal = bSrc;

	return *this;
}

CDCIData& CDCIData::operator=( short sSrc )
{
	this->vt = VT_UI2;
	this->iVal = sSrc;

	return *this;
}

CDCIData& CDCIData::operator=( long lSrc )
{
	this->vt = VT_UI4;
	this->lVal = lSrc;

	return *this;
}

CDCIData& CDCIData::operator=( float fltSrc )
{
	this->vt =  VT_R4;
	this->fltVal = fltSrc;

	return *this;
}

CDCIData& CDCIData::operator=( double dSrc )
{
	this->vt = VT_R8;
	this->dblVal = dSrc;

	return *this;
}

CDCIData& CDCIData::operator=( LPCTSTR pSrc )
{
	this->vt = VT_BSTR;
	this->bstrVal = (_bstr_t)pSrc;

	return *this;
}

CDCIData& CDCIData::operator=( const _variant_t& vSrc )
{
	this->vt = vSrc.vt;
	switch(vSrc.vt)
	{
	case VT_I2:
		this->iVal = vSrc.iVal;
		break;
	case VT_I4:
		this->lVal = vSrc.lVal;
		break;
	case VT_R4:
		this->fltVal = vSrc.fltVal;
		break;
	case VT_R8:
		this->dblVal = vSrc.dblVal;
		break;
	case  VT_BOOL:
		this->boolVal = vSrc.boolVal;
		break;
	case VT_UI1:
		this->bVal = vSrc.bVal;
		break;
	case  VT_BSTR:
		this->bstrVal = vSrc.bstrVal;
		break;
	case VT_DATE:
		this->date = vSrc.date;
		break;
	case  VT_NULL:
		this->vt = VT_NULL;
		break;
	}

	return *this;
}

CDCIData& CDCIData::operator=( const CDCIData& vSrc )
{
	this->vt = vSrc.vt;
	switch(vSrc.vt)
	{
	case VT_I2:
		this->iVal = vSrc.iVal;
		break;
	case VT_I4:
		this->lVal = vSrc.lVal;
		break;
	case VT_R4:
		this->fltVal = vSrc.fltVal;
		break;
	case VT_R8:
		this->dblVal = vSrc.dblVal;
		break;
	case  VT_BOOL:
		this->boolVal = vSrc.boolVal;
		break;
	case VT_UI1:
		this->bVal = vSrc.bVal;
		break;
	case  VT_BSTR:
		this->bstrVal = vSrc.bstrVal;
		break;
	case VT_DATE:
		this->date = vSrc.date;
		break;
	case  VT_NULL:
		this->vt = VT_NULL;
		break;
	}

	return *this;
}

CDCIData& CDCIData::operator=( int nSrc )
{
	this->vt = VT_INT;
	this->intVal = nSrc;

	return *this;
}

//CDCIData& CDCIData::operator=( const CString& strSrc )
//{
//	this->vt = VT_BSTR;
//	this->bstrVal = (_bstr_t)strSrc;
//
//	return *this;
//}

CDCIData& CDCIData::operator=(const COleDateTime& tSrc)
{
	this->vt = VT_DATE;
	this->date = tSrc;

	return *this;
}

CDCIData& CDCIData::operator=( const CTime& tSrc )
{
	time_t tm = tSrc.GetTime();
	COleDateTime oleTime(tm);

	*this = oleTime;

	return *this;
}

long CDCIData::Long_S( bool bResult /*= NULL*/, long lDefault /*= -1*/ ) const
{
	long 	lResult = lDefault;
	switch (this->vt)
	{
	case   VT_BOOL: //bool
		{
			lResult  =  this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			lResult   =   this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			lResult   =   this->cVal;
			bResult = true;
		}
		break; 
	case   VT_I2: //short
	case   VT_UI2: //unsigned short
		{
			lResult   =   this->iVal;
			bResult = true;
		}
		break; 
	case   VT_I4: //long
	case   VT_UI4: //unsigned long
		{
			lResult   =   this->lVal;
			bResult = true;
		}
		break; 
	case   VT_INT: //int
		{
			lResult   =   this->intVal;
			bResult = true;
		}
		break; 
	case   VT_R4: //float
		{
			lResult   =   (long)(this->fltVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			lResult   =   (long)(this->dblVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			lResult   =   (long)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			lResult   =   (long)this->vt;
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			lResult   =   _ttol((LPCTSTR)(_bstr_t)this->vt); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
		break;
	}
	return lResult;
}

double CDCIData::Double_S( bool bResult /*= NULL*/, double dDefault /*= 0.0*/ ) const
{
	double   dResult   =   dDefault; 
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch   (this->vt) 
	{ 
	case   VT_R4: //float
		{
			dResult   =   this->fltVal; 
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			dResult   =   this->dblVal; 
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			dResult   =   (double)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			dResult   =   (double)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_BOOL: //bool
		{
			dResult   =   (double)this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			dResult   =   (double)this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			dResult = (double)this->cVal;
			bResult = true;
		}
		break;
	case   VT_UI2: //unsigned short
	case   VT_I2: //shourt
		{
			dResult   =   (double)this->iVal; 
			bResult = true;
		}
		break; 
	case   VT_UI4: //unsigned long
	case   VT_I4: //long
		{
			dResult   =   (double)this->lVal; 
			bResult = true;
		}
		break; 
	case   VT_INT: //int
		{
			dResult   =   (double)this->intVal; 
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			dResult   =   _tstof((LPCTSTR)(_bstr_t)this); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
	} 
	return dResult;
}

CString CDCIData::String_S( bool bResult /*= NULL*/, LPCTSTR lpszDefault /*= NULL*/ ) const
{
	CString   strValue = lpszDefault;
	if (bResult != NULL)
	{
		bResult = false;
	}

	switch   (this->vt) 
	{ 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			strValue   =   (LPCTSTR)(_bstr_t)this;
			bResult = true;
		}
		break; 
	case   VT_I1: 
		{
			strValue.Format( _T("%c "),  this->cVal);
			bResult = true;
		}
		break; 
	case   VT_UI1: 
		{
			strValue.Format( _T("%c "),  this->bVal);
			bResult = true;
		}
		break; 
	case   VT_I2://短整型 
		{
			strValue.Format(_T( "%d "),   this->iVal); 
			bResult = true;
		}
		break; 
	case   VT_UI2://无符号短整型 
		{
			strValue.Format(_T( "%d "),   this->uiVal); 
			bResult = true;
		}
		break; 
	case   VT_INT://整型 
		{
			strValue.Format(_T( "%d "),   this->intVal);
			bResult = true;
		}
		break; 
	case   VT_I4:   //整型 
		{
			strValue.Format(_T( "%d "),   this->lVal); 
			bResult = true;
		}
		break; 
	case   VT_I8:   //长整型 
		{
			strValue.Format( _T("%ld "),   this->lVal); 
			bResult = true;
		}
		break; 
	case   VT_UINT://无符号整型 
		{
			strValue.Format( _T("%ld "),   this->uintVal); 
			bResult = true;
		}
		break; 
	case   VT_UI4:   //无符号整型 
		{
			strValue.Format(_T( "%d "),   this->ulVal); 
			bResult = true;
		}
		break; 
	case   VT_UI8:   //无符号长整型 
		{
			strValue.Format(_T( "%d "),   this->ulVal); 
			bResult = true;
		}
		break; 
	case   VT_VOID: 
		{
			strValue.Format( _T("%8x "),   this->byref); 
			bResult = true;
		}
		break; 
	case   VT_R4://浮点型 
		{
			strValue.Format( _T("%.4f "),   this->fltVal); 
			bResult = true;
		}
		break; 
	case   VT_R8://双精度型 
		{
			strValue.Format( _T("%.8f "),   this->dblVal); 
			bResult = true;
		}
		break; 
	case   VT_DECIMAL:   //小数 
		{
			strValue.Format( _T("%.8f "),   this->decVal); 
			bResult = true;
		}
		break; 
	case   VT_CY: 
		{ 
			COleCurrency   cy   =   this->cyVal; 
			strValue   =   cy.Format(); 
			bResult = true;
		} 
		break; 
	case   VT_BLOB: 
	case   VT_BLOB_OBJECT: 
		{
			strValue   =  _T("[BLOB] ") ; 
			bResult = true;
		}
		break; 
	case   VT_BOOL://布尔型 
		{
			strValue = this->boolVal ? _T("true ") : _T("false ") ; 
			bResult = true;
		}
		break; 
	case   VT_DATE:   //日期型 
		{ 
			DATE   dt   =   this->date; 
			COleDateTime  da = COleDateTime(dt);   
			strValue = da.Format(_T("%Y-%m-%d   %H:%M:%S "));
			bResult = true;
		} 
		break; 
	case   VT_NULL://NULL值 
	case   VT_EMPTY://空 
		{
			strValue  =  _T("") ; 
			bResult = true;
		}
		break; 
	case   VT_UNKNOWN://未知类型 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
		break; 
	} 
	return   strValue; 
}

short CDCIData::Short_S( bool bResult /*= NULL*/, short nDefault /*= -1*/ ) const
{
	short nResult = nDefault;
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch (this->vt)
	{
	case   VT_BOOL: //bool
		{
			nResult  =  this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			nResult   =   this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			nResult   =   this->cVal;
			bResult = true;
		}
		break; 
	case   VT_I2: //short
	case   VT_UI2: //unsigned short
		{
			nResult   =   this->iVal;
			bResult = true;
		}
		break; 
	case   VT_I4: //long
	case   VT_UI4: //unsigned long
		{
			nResult   =   this->lVal;
			bResult = true;
		}
		break; 
	case   VT_INT: //int
		{
			nResult   =   this->intVal;
			bResult = true;
		}
		break; 
	case   VT_R4: //float
		{
			nResult   =   (int)(this->fltVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			nResult   =   (int)(this->dblVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			nResult   =   (int)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			nResult   =   (int)this->vt;
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			nResult   =   _ttoi((LPCTSTR)(_bstr_t)this->vt); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
		break;
	}

	return nResult;
}

BYTE CDCIData::Byte_S( bool bResult /*= NULL*/, BYTE bDefault /*= 0*/ ) const
{
	BYTE nResult = bDefault;
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch (this->vt)
	{
	case   VT_BOOL: //bool
		{
			nResult  =  this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			nResult   =   this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			nResult   =   this->cVal;
			bResult = true;
		}
		break; 
	case   VT_I2: //short
	case   VT_UI2: //unsigned short
		{
			nResult   =   this->iVal;
			bResult = true;
		}
		break; 
	case   VT_I4: //long
	case   VT_UI4: //unsigned long
		{
			nResult   =   this->lVal;
			bResult = true;
		}
		break; 
	case   VT_INT: //int
		{
			nResult   =   this->intVal;
			bResult = true;
		}
		break; 
	case   VT_R4: //float
		{
			nResult   =   (int)(this->fltVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			nResult   =   (int)(this->dblVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			nResult   =   (int)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			nResult   =   (int)this->vt;
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			nResult   =   _ttoi((LPCTSTR)(_bstr_t)this->vt); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
		break;
	}


	return nResult;
}

bool CDCIData::Boolean_S( bool bResult /*= NULL*/, bool bDefault /*= false*/ ) const
{
	bool   bDefaultRes   =   bDefault; 
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch   (this->vt) 
	{ 
	case   VT_BOOL: 
		{
			bDefaultRes   =   (this->boolVal) ? true : false; 
			bResult = true;
		}
	case   VT_EMPTY: 
	case   VT_NULL: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
	} 
	return bDefaultRes;
}

float CDCIData::Float_S( bool bResult /*= NULL*/, float fDefault /*= 0.0f*/ ) const
{
	float fResult = fDefault;
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch   (this->vt) 
	{ 
	case   VT_R4: //float
		{
			fResult   =   this->fltVal; 
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			fResult   =   this->dblVal; 
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			fResult   =   (float)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			fResult   =   (float)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_BOOL: //bool
		{
			fResult   =   (float)this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			fResult   =   (float)this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			fResult = (float)this->cVal;
			bResult = true;
		}
		break;
	case   VT_UI2: //unsigned short
	case   VT_I2: //shourt
		{
			fResult   =   (float)this->iVal; 
			bResult = true;
		}
		break; 
	case   VT_UI4: //unsigned long
	case   VT_I4: //long
		fResult   =   (float)this->lVal; 
		break; 
	case   VT_INT: //int
		{
			fResult   =   (float)this->intVal; 
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			fResult   =   _tstof((LPCTSTR)(_bstr_t)this->vt); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
	} 
	return fResult;
}

COleDateTime CDCIData::OleTime_S( bool bResult /*= NULL*/ ) const
{
	COleDateTime   OleResult; 
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch   (this->vt)   
	{ 
	case   VT_DATE: 
		{ 
			OleResult   =   this->date; 
			bResult = true;
		} 
		break; 
	case   VT_EMPTY: 
	case   VT_NULL: 
		{
			OleResult.SetStatus(COleDateTime::null); 
			bResult = true;
		}
		break; 
	default: 
		bResult = false;
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
	} 
	return   OleResult; 
}

CTime CDCIData::Time_S( bool bResult /*= NULL*/ ) const
{
	COleDateTime   OleResult; 
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch   (this->vt)   
	{ 
	case   VT_DATE: 
		{ 
			OleResult   =   this->date; 
			bResult = true;
		} 
		break; 
	case   VT_EMPTY: 
	case   VT_NULL: 
		{
			OleResult.SetStatus(COleDateTime::null); 
			bResult = true;
		}
		break; 
	default: 
		bResult = false;
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
	} 

	SYSTEMTIME sysTime;
	OleResult.GetAsSystemTime(sysTime);

	CTime myTime(sysTime);

	return myTime;
}

int CDCIData::Int_S( bool bResult /*= NULL*/, int nDefault /*= -1*/ ) const
{
	int nResult = nDefault;
	if (bResult != NULL)
	{
		bResult = false;
	}
	switch (this->vt)
	{
	case   VT_BOOL: //bool
		{
			nResult  =  this->boolVal; 
			bResult = true;
		}
		break; 
	case   VT_UI1: //BYTE
		{
			nResult   =   this->bVal; 
			bResult = true;
		}
		break; 
	case   VT_I1: //char
		{
			nResult   =   this->cVal;
			bResult = true;
		}
		break; 
	case   VT_I2: //short
	case   VT_UI2: //unsigned short
		{
			nResult   =   this->iVal;
			bResult = true;
		}
		break; 
	case   VT_I4: //long
	case   VT_UI4: //unsigned long
		{
			nResult   =   this->lVal;
			bResult = true;
		}
		break; 
	case   VT_INT: //int
		{
			nResult   =   this->intVal;
			bResult = true;
		}
		break; 
	case   VT_R4: //float
		{
			nResult   =   (int)(this->fltVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_R8: //double
		{
			nResult   =   (int)(this->dblVal   +   0.5);
			bResult = true;
		}
		break; 
	case   VT_DECIMAL: //decimal
		{
			nResult   =   (int)this->vt; 
			bResult = true;
		}
		break; 
	case   VT_CY: //CY
		{
			nResult   =   (int)this->vt;
			bResult = true;
		}
		break; 
	case   VT_BSTR://字符串 
	case   VT_LPSTR://字符串 
	case   VT_LPWSTR://字符串 
		{
			nResult   =   _ttoi((LPCTSTR)(_bstr_t)this->vt); 
			bResult = true;
		}
		break; 
	case   VT_NULL: 
	case   VT_EMPTY: 
		bResult = true;
		break; 
	default: 
		TRACE(_T( "Warning:   未处理的   _variant_t   类型值;   文件:   %s;   行:   %d\n "),   __FILE__,   __LINE__); 
		break;
	}

	return nResult;
}
