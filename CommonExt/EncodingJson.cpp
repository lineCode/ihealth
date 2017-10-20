#include "stdafx.h"
#include "EncodingJson.h"

using namespace Ext::CPP;

CEncodingJson::CEncodingJson(void)
{
}

CEncodingJson::~CEncodingJson(void)
{
}

CString	CEncodingJson::Marshal(const Interface& value)
{
	CEncodingJson json;
	return json.encode(value);
}

Interface CEncodingJson::Unmarshal(const CString &jsonValue)
{
	Interface o;
	
	CEncodingJson json;
	json.decode(jsonValue, o);

	return o;
}

CString CEncodingJson::encode(const Interface& value)
{
	if (value.GetType() != Interface::ObjectType)
	{
		return _T("{}");
	}

	return encodeObject(value);
}

void CEncodingJson::decode(const TCHAR *pszJsonValue, Interface& o)
{
	int iCount = getKVCount(pszJsonValue);
	for (int i = 0; i < iCount; i++)
	{
		CString k = decodeCString(getKAt(pszJsonValue, i));
		CString strValue = getVAt(pszJsonValue, i);
		Interface v;
		// Interface tmp;
		// o.Insert(k, tmp);		
		// Interface& v = o[k]
		switch(decodeVType(strValue))
		{
		case CEncodingJson::BoolType:
			v = decodeBool(strValue);
			break;
		case CEncodingJson::Int64Type:
			v = decodeInt64(strValue);
			break;
		case CEncodingJson::DoubleType:
			v = decodeDouble(strValue);
			break;
		case CEncodingJson::StringType:
			v = decodeCString(strValue);
			break;
		case CEncodingJson::ArrayType:
			decodeArray(strValue, v);
			break;
		case CEncodingJson::ObjectType:
			decodeObject(strValue, v);
			break;
		default:
			break;
		}

		o.Insert(k, v);
	}
}

int	CEncodingJson::getKVCount(const TCHAR* pszJsonValue)
{
	int iLen = (int)_tcslen(pszJsonValue);
	if (pszJsonValue == NULL || iLen < 1)
	{
		return 0;
	}
	int iCount = 0;
	TCHAR cCur = 0;
	TCHAR cPrev = 0;
	int iLevel = 0;
	bool bContent = false;	
	
	for (int i = 0; i < iLen; i++)
	{
		cCur = pszJsonValue[i];
		if(i != 0) 
		{
			cPrev = pszJsonValue[i - 1];
		}
		if (cCur == 34 && cPrev != 92)
		{
			bContent = !bContent;
		}
		if (bContent)
		{
			continue;
		}

		if (cCur == 123 || cCur == 91)
		{
			iLevel++;
		}
		if (cCur == 125 || cCur == 93)
		{
			iLevel--;
		}

		if (iLevel == 1 && cCur == 58)
		{
			iCount++;
		}
	}

	return iCount;
}

CString CEncodingJson::getKAt(const TCHAR* pszJsonValue, int iIndex)
{
	CString k = _T("");
	int iLen = (int)_tcslen(pszJsonValue);
	if (iLen < 1)
	{
		return k;
	}
	int iCount = 0;
	TCHAR cCur = 0;
	TCHAR cPrev = 0;
	int iLevel = 0;
	bool bContent = false;	
	bool bKey = false;
	for (int i = 0; i < iLen; i++)
	{
		cCur = pszJsonValue[i];
		if(i != 0) 
		{
			cPrev = pszJsonValue[i - 1];
		}
		if (bKey)
		{
			k += cCur;
		}
		if (cCur == 34 && cPrev != 92)
		{
			bContent = !bContent;
		}
		if (bContent)
		{
			continue;
		}

		if (cCur == 123 || cCur == 91)
		{
			iLevel++;
		}
		if (cCur == 125 || cCur == 93)
		{
			iLevel--;
		}

		if (iLevel == 1 && cCur == 58)
		{
			iCount++;
		}
		
		if (iLevel == 1 && (cCur == 123 || cCur == 91 || cCur == 44) && iCount == iIndex)
		{
			bKey = true;
		}

		if (iLevel == 1 && cCur == 58 && bKey)
		{
			break;
		}
	}

	k = k.Left(k.ReverseFind(58));
	return k;
}

CString CEncodingJson::getVAt(const TCHAR* pszJsonValue, int iIndex)
{
	CString v = _T("");
	int iLen = (int)_tcslen(pszJsonValue);
	if (iLen < 1)
	{
		return v;
	}
	int iCount = 0;
	TCHAR cCur = 0;
	TCHAR cPrev = 0;
	int iLevel = 0;
	bool bContent = false;	
	bool bValue = false;

	for (int i = 0; i < iLen; i++)
	{
		cCur = pszJsonValue[i];
		if (bValue)
		{
			v += cCur;
		}
		if(i != 0) 
		{
			cPrev = pszJsonValue[i - 1];
		}
		if (cCur == 34 && cPrev != 92)
		{
			bContent = !bContent;
		}
		if (bContent)
		{
			continue;
		}
		if (cCur == 123 || cCur == 91)
		{
			iLevel++;
		}
		if (cCur == 125 || cCur == 93)
		{
			iLevel--;
		}

		if (iLevel == 1 && cCur == 58)
		{
			iCount++;
		}
		if (iCount == (iIndex + 1) && cCur == 58 && iLevel == 1)
		{
			bValue = true;
		}

		if (iLevel == 1 && cCur == 44 && iCount == (iIndex + 1))
		{
			break;
		}
	}

	int iPos = 0;
	iLen = v.GetLength() - 1;
	for (; iLen > -1; iLen--)
	{
		if (v.GetAt(iLen) < 33 || v.GetAt(iLen) == 127)
		{
			continue;
		}
		break;
	}

	v = v.Left(iLen);
	return v;
}


int CEncodingJson::getArrayCount(const TCHAR *pszJsonValue)
{
	int iCount = 0;
	TCHAR cPrev = 0;
	TCHAR cCur = 0;
	int iLevel = 0;
	bool bContent = false;
	int iLen = (int)_tcslen(pszJsonValue);
	bool bNull = true;

	for (int i = 0; i < iLen; i++)
	{
		cCur = pszJsonValue[i];
		if (i != 0)
		{
			cPrev = pszJsonValue[i - 1];
		}

		if (cCur > 32 && cCur != 127)
		{
			bNull = false;
		}
		if (cCur == 34 && cPrev != 92)
		{
			bContent = !bContent;
		}

		if (bContent)
		{
			continue;
		}

		if (cCur == 91 || cCur == 123)
		{
			iLevel++;
		}
		if (cCur == 93 || cCur == 125)
		{
			iLevel--;
		}

		if (iLevel == 1 && cCur == 44)
		{
			iCount++;
		}
	}

	return bNull ? iCount:(iCount + 1);
}

CString CEncodingJson::getArrayAt(const TCHAR* pszJsonValue, int iIndex)
{
	CString v;
	int iCount = 0;
	TCHAR cPrev = 0;
	TCHAR cCur = 0;
	int iLevel = 0;
	bool bContent = false;
	int i = 0;
	bool bValue = false;
	int iLen = (int)_tcslen(pszJsonValue);
	for (; i < iLen; i++)
	{
		if (iCount == iIndex)
		{
			bValue = true;
		}
		cCur = pszJsonValue[i];
		if (i != 0)
		{
			cPrev = pszJsonValue[i - 1];
		}

		if (cCur == 34 && cPrev != 92)
		{
			bContent = !bContent;
		}

		if (bContent && !bValue)
		{
			continue;
		}
		if (bContent && bValue)
		{
			v += cCur;
			continue;
		}

		// 非字符串
		if (iLevel != 0 && bValue)
		{
			v += cCur;
		}
		if (cCur == 91 || cCur == 123)
		{
			iLevel++;
		}
		if (cCur == 93 || cCur == 125)
		{
			iLevel--;
		}

		if (iLevel == 1 && cCur == 44)
		{
			if (v.GetLength() > 0)
			{
				break;
			}
			iCount++;
		}
	}

	iLen = v.GetLength() - 1;
	for (; iLen > -1; iLen--)
	{
		if (v.GetAt(iLen) < 33 || v.GetAt(iLen) == 127)
		{
			continue;
		}
		break;
	}
	v = v.Left(iLen);

	return v;
}

int CEncodingJson::decodeVType(const TCHAR* pszValue)
{
	int iLen = (int)_tcslen(pszValue);
	TCHAR cCur = 0;
	TCHAR cPrev = 0;

	if (_tcscmp(pszValue, _T("true")) == 0 || 
		_tcscmp(pszValue, _T("false")) == 0)
	{
		return BoolType;
	}

	// golang 的 jsonrpc 以 null 做为空错误
	if (_tcscmp(pszValue, _T("null")) == 0)
		return StringType;

	for (int i = 0; i < iLen; i++)
	{
		cCur = pszValue[i];
		if (i != 0)
		{
			cPrev = pszValue[i - 1];
		}

		if (cCur < 33 || cCur > 126)
		{
			continue;
		}

		if (cCur == 34 && cPrev != 92)
		{
			return StringType;
		}
		else if (cCur == 123)
		{
			return ObjectType;
		}
		else if (cCur == 91)
		{
			return ArrayType;
		}
		else if (cCur == 46)
		{
			return DoubleType;
		}
	}

	return Int64Type;
}

Interface CEncodingJson::decodeBool(const TCHAR* pszJsonValue)
{
	Interface v;
	if (_tcscmp(pszJsonValue, _T("true")) == 0)
	{
		v = true;
	}
	else 
	{
		v = false;
	}
	return v;
}

Interface CEncodingJson::decodeInt64(const TCHAR* pszJsonValue)
{
	Interface v;
	v = _wtol(pszJsonValue);
	return v;
}

Interface CEncodingJson::decodeDouble(const TCHAR* pszJsonValue)
{
	Interface v;
	v = _wtof(pszJsonValue);
	return v;
}

Interface CEncodingJson::decodeCString(const TCHAR* pszJsonValue)
{
	Interface v;
	CString strValue;
	int iLen = (int)_tcslen(pszJsonValue);
	TCHAR cCur = 0;
	int iBegin = 0;
	for (; iBegin < iLen; iBegin++)
	{
		cCur = pszJsonValue[iBegin];
		if (cCur < 33 || cCur == 127)
		{
			continue;
		}

		if (cCur == 34)
		{
			iBegin++;
			break;
		}
		else 
		{
			break;
		}
	}

	int iEnd = iLen - 1;
	for (; iEnd > -1; iEnd--)
	{
		cCur = pszJsonValue[iEnd];
		if (cCur < 33 || cCur == 127)
		{
			continue;
		}
		if (cCur == 34)
		{
			iEnd--;
			break;
		}
		else 
		{
			break;
		}
	}

	for (int i = iBegin; i <= iEnd; i++)
	{
		strValue += pszJsonValue[i];
	}

	v = strValue;
	return v;
}

void CEncodingJson::decodeArray(const TCHAR* pszJsonValue, Interface& v)
{
	int iLen = getArrayCount(pszJsonValue);
	for (int i = 0; i < iLen; i++)
	{
		CString strV = getArrayAt(pszJsonValue, i);
		Interface item;

		switch(decodeVType(strV))
		{
		case Int64Type:
			item = decodeInt64(strV);
			break;
		case DoubleType:
			item = decodeDouble(strV);
			break;
		case StringType:
			item = decodeCString(strV);
			break;
		case ArrayType:
			decodeArray(strV, item);
			break;
		case ObjectType:
			decodeObject(strV, item);
			break;
		default:
			break;
		}

		v.Append(item);
	}
}

void CEncodingJson::decodeObject(const TCHAR* pszJsonValue, Interface& v)
{
	decode(pszJsonValue, v);
}

CString CEncodingJson::encodeBool(const Interface& value)
{
	CString v = _T("");

	bool bValue = value;
	if (bValue)
	{
		v = _T("true");
	}
	else
	{
		v = _T("false");
	}

	return v;
}

CString CEncodingJson::encodeInt64(const Interface& value)
{
	CString v = _T("");
	
	long lv = 0;
	if (value.GetType() == Interface::IntType)
	{
		lv = (int)value;
	}
	else if (value.GetType() == Interface::LongType)
	{
		lv = value;
	}
	else if (value.GetType() == Interface::BoolType)
	{
		lv = (bool)value;
	}
	else if (value.GetType() == Interface::Uint64Type)
	{
		lv = (UINT64)value;
	}
	else 
	{
		return v;
	}

	v.Format(_T("%d"), lv);

	return v;
}

CString CEncodingJson::encodeDouble(const Interface& value)
{
	CString v = _T("");
	
	double dv = 0.0;
	if (value.GetType() == Interface::DoubleType)
	{
		dv = value;
	}

	v.Format(_T("%f"), dv);

	return v;
}

CString CEncodingJson::encodeCString(const Interface& value)
{
	CString v = _T("");
	
	if (value.GetType() != Interface::StringType)
	{
		return v;
	}

	v = value;
	v = _T("\"") + v + _T("\"");
	
	v.Replace(_T("\\\\"), _T("\\"));  // golang decode 不支持\\格式路径，路径统一采用/
	v.Replace('\\', '/');
	return v;
}

CString CEncodingJson::encodeArray(const Ext::CPP::Interface &value)
{
	CString v = _T("");
	
	if (value.GetType() != Interface::ArrayType)
	{
		return v;
	}

	int isize = value.Count();
	v += _T("[");
	for (int i = 0; i < isize; i++)
	{
		Interface iv = value.At(i);
		
		switch(iv.GetType())
		{
		case Interface::BoolType:
			v += encodeBool(iv);
			break;
		case Interface::IntType:
		case Interface::Uint64Type:
		case Interface::LongType:
			v += encodeInt64(iv);
			break;
		case Interface::DoubleType:
			v += encodeDouble(iv);
			break;
		case Interface::StringType:
			v += encodeCString(iv);
			break;
		case Interface::ArrayType:
			v += encodeArray(iv);
			break;
		case Interface::ObjectType:
			v += encodeObject(iv);
			break;
		default:
			break;
		}

		if (i == (isize - 1))
		{
			continue;
		}
		v += _T(",");
	}
	v += _T("]");

	return v;
}

CString CEncodingJson::encodeObject(const Ext::CPP::Interface &value)
{
	CString v = _T("");
	bool	bStart = true;
	if (value.GetType() != Interface::ObjectType)
	{
		return v;
	}

	v += _T("{");
	for (value.Start(); value.KeepUp(); value.Step())
	{
		Element& e = value.Current();
		if (!bStart)
		{
			v += _T(",");
		}
		CString strK = e.k;
		CString strV = _T("");
		Interface ifV = e.v;

		switch(ifV.GetType())
		{
		case Interface::BoolType:
			strV += encodeBool(ifV);
			break;
		case Interface::IntType:
		case Interface::Uint64Type:
		case Interface::LongType:
			strV += encodeInt64(ifV);
			break;
		case Interface::DoubleType:
			strV += encodeDouble(ifV);
			break;
		case Interface::StringType:
			strV += encodeCString(ifV);
			break;
		case Interface::ArrayType:
			strV += encodeArray(ifV);
			break;
		case Interface::ObjectType:
			strV += encodeObject(ifV);
			break;
		default:
			break;
		}

		v += encodeCString(strK) + _T(":") + strV;
		bStart = false;
	}

	v += _T("}");

	return v;
}