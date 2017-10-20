#include "StdAfx.h"
#include "InterfaceEx.h"
#include <map>
#include <list>

using namespace std;

namespace Ext
{
	namespace CPP
	{
		InterfaceEx::InterfaceEx(void)
		{
			m_pIter = NULL;
			m_pData = NULL;
			m_iType = NullType;
		}

		InterfaceEx::~InterfaceEx(void)
		{
			Clear();
			m_pData = NULL;
			m_iType = NullType;
		}

		InterfaceEx::InterfaceEx(const InterfaceEx& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const bool& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const int& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const UINT64& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const long& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const double& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const TCHAR* val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx::InterfaceEx(const CString& val)
		{
			m_pIter = NULL;
			m_pData = NULL;
			*this = val;
		}

		InterfaceEx& InterfaceEx::operator=(const InterfaceEx& val)
		{
			m_iType = val.m_iType;

			if (m_iType == BoolType)
			{
				m_pData = new bool;
				*((bool*)m_pData) = *((bool* const)(val.m_pData));
			}
			else if (m_iType == IntType)
			{
				m_pData = new int;
				*((int*)m_pData) = *((int* const)(val.m_pData));
			}
			else if (m_iType == Uint64Type)
			{
				m_pData = new UINT64;
				*((UINT64*)m_pData) = *((UINT64* const)(val.m_pData));
			}
			else if (m_iType == LongType)
			{
				m_pData = new long;
				*((long*)m_pData) = *((long* const)(val.m_pData));
			}
			else if (m_iType == DoubleType)
			{
				m_pData = new double;
				*((double*)m_pData) = *((double* const)(val.m_pData));
			}
			else if (m_iType == StringType)
			{
				m_pData = new CString;
				*((CString*)m_pData) = *((CString* const)(val.m_pData));
			}
			else if (m_iType == ArrayType)
			{
				m_pData = new list<InterfaceEx>;
				*((list<InterfaceEx>*)m_pData) = *((list<InterfaceEx>* const)(val.m_pData));
			}
			else if (m_iType == ObjectType)
			{
 				m_pData = new map<CString, InterfaceEx>;
 				*((map<CString, InterfaceEx>*)m_pData) = *((map<CString, InterfaceEx>* const)(val.m_pData));
			}

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const bool& val)
		{
			SetBoolVal(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const int& val)
		{
			SetIntVal(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const UINT64& val)
		{
			SetUint64Val(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const long& val)
		{
			SetLongVal(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const double& val)
		{
			SetDoubleVal(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const TCHAR* val)
		{
			SetStringVal(val);

			return *this;
		}

		InterfaceEx& InterfaceEx::operator=(const CString& val)
		{
			SetStringVal(val);

			return *this;
		}

		InterfaceEx::operator bool&() const
		{
			return GetBoolVal();
		}

		InterfaceEx::operator int&() const
		{
			return GetIntVal();
		}

		InterfaceEx::operator UINT64&() const
		{
			return GetUint64Val();
		}

		InterfaceEx::operator long&() const
		{
			return GetLongVal();
		}

		InterfaceEx::operator double&() const
		{
			return GetDoubleVal();
		}

		InterfaceEx::operator CString&() const
		{
			return GetStringVal();
		}

		bool& InterfaceEx::GetBoolVal() const
		{
			return *(bool*)m_pData;
		}

		int& InterfaceEx::GetIntVal() const
		{
			if (m_iType == LongType)
			{
				return (int&)GetLongVal();
			}
			else if (m_iType == DoubleType)
			{
				return (int&)GetDoubleVal();
			}
			else if (m_iType == Uint64Type)
			{
				return (int&)GetUint64Val();
			}
			
			return *((int*)m_pData);
		}

		UINT64& InterfaceEx::GetUint64Val() const
		{
			if (m_iType == LongType)
			{
				return (UINT64&)GetLongVal();
			}
			else if (m_iType == DoubleType)
			{
				return (UINT64&)GetDoubleVal();
			}
			else if (m_iType == IntType)
			{
				return (UINT64&)GetIntVal();
			}

			return *((UINT64*)m_pData);
		}

		long& InterfaceEx::GetLongVal() const
		{
			if (m_iType == Uint64Type)
			{
				return (long&)GetUint64Val();
			}
			else if (m_iType == DoubleType)
			{
				return (long&)GetDoubleVal();
			}
			else if (m_iType == IntType)
			{
				return (long&)GetIntVal();
			}

			return *((long*)m_pData);
		}

		double& InterfaceEx::GetDoubleVal() const
		{
			if (m_iType == LongType)
			{
				return (double&)GetLongVal();
			}
			else if (m_iType == Uint64Type)
			{
				return (double&)GetUint64Val();
			}
			else if (m_iType == IntType)
			{
				return (double&)GetIntVal();
			}

			return *((double*)m_pData);
		}

		CString& InterfaceEx::GetStringVal() const
		{
			return *((CString*)m_pData);
		}

		void InterfaceEx::SetBoolVal(const bool& val) const
		{
			Clear();

			bool *p = new bool;
			*p = val;
			m_pData = p;

			m_iType = BoolType;
		}

		void InterfaceEx::SetIntVal(const int& val) const
		{
			Clear();

			int *p = new int;
			*p = val;	
			m_pData = p;

			m_iType = IntType;
		}

		void InterfaceEx::SetUint64Val(const UINT64& val) const
		{
			Clear();


			UINT64 *p = new UINT64;
			*p = val;
			m_pData = p;

			m_iType = Uint64Type;
		}

		void InterfaceEx::SetLongVal(const long& val) const
		{
			Clear();

			long *p = new long;
			*p = val;
			m_pData = p;

			m_iType = LongType;
		}

		void InterfaceEx::SetDoubleVal(const double& val) const
		{
			Clear();

			double *p = new double;
			*p = val;
			m_pData = p;

			m_iType = DoubleType;
		}

		void InterfaceEx::SetStringVal(const CString& val) const
		{
			Clear();

			CString *p = new CString;
			*p = val;
			m_pData = p;

			m_iType = StringType;
		}

		void InterfaceEx::Insert(const CString& k, const InterfaceEx& v) const
		{
			map<CString, InterfaceEx>* p = NULL;
			if (m_iType != ObjectType || m_pData == NULL)
			{
				Clear();

				p = new map<CString, InterfaceEx>;
				m_pData = p;
				m_iType = ObjectType;
			}
			p = (map<CString, InterfaceEx>*)m_pData;
			p->insert(make_pair(k, v));
		}

		void InterfaceEx::Delete(const CString& k) const
		{
			map<CString, InterfaceEx>* p = NULL;
			if (m_iType != ObjectType || m_pData == NULL)
			{
				Clear();

				p = new map<CString, InterfaceEx>;
				m_pData = p;
				m_iType = ObjectType;
			}
			p = (map<CString, InterfaceEx>*)m_pData;
			p->erase(k);
		}

		void InterfaceEx::Update(const CString& k, const InterfaceEx& v) const
		{
			map<CString, InterfaceEx>* p = NULL;
			if (m_iType != ObjectType || m_pData == NULL)
			{
				Clear();

				p = new map<CString, InterfaceEx>;
				m_pData = p;
				m_iType = ObjectType;
			}
			p = (map<CString, InterfaceEx>*)m_pData;
			(*p)[k] = v;
		}

		InterfaceEx& InterfaceEx::Query(const CString& k) const
		{
			map<CString, InterfaceEx>* p = NULL;
			if (m_iType != ObjectType || m_pData == NULL)
			{
				Clear();

				p = new map<CString, InterfaceEx>;
				m_pData = p;
				m_iType = ObjectType;
			}
			p = (map<CString, InterfaceEx>*)m_pData;
			map<CString, InterfaceEx>::iterator iter = p->find(k);
			if (iter == p->end())
			{
				InterfaceEx v;
				p->insert(make_pair(k, v));
			}

			return (*p)[k];
		}

		InterfaceEx& InterfaceEx::operator [](const CString& k) const
		{
			return Query(k);
		}

		InterfaceEx& InterfaceEx::operator [](const wchar_t* k) const
		{
			return Query(k);
		}

		Element InterfaceEx::First() const
		{
			Element v;

			if (m_pIter != NULL)
			{
				delete m_pIter;
				m_pIter = NULL;
			}
			if (m_iType == ObjectType && m_pData != NULL)
			{
				map<CString, InterfaceEx>::iterator* pIter = new map<CString, InterfaceEx>::iterator;
				map<CString, InterfaceEx>* p = (map<CString, InterfaceEx>*)m_pData;
				*pIter = p->begin();
				m_pIter = pIter;

				if (!End())
				{
					v.k = (*pIter)->first;
					v.v = (*pIter)->second;
				}
			}
			else if (m_iType == ArrayType && m_pData != NULL)
			{
				list<InterfaceEx>::iterator* pIter = new list<InterfaceEx>::iterator;
				list<InterfaceEx>* p = (list<InterfaceEx>*)m_pData;
				*pIter = p->begin();
				m_pIter = pIter;

				if (!End())
				{
					v.v = **pIter;
				}
			}

			return v;
		}

		Element InterfaceEx::Next() const
		{
			Element v;
			
			if (m_pIter != NULL && m_iType == ObjectType)
			{
				map<CString, InterfaceEx>::iterator* pIter = (map<CString, InterfaceEx>::iterator*)m_pIter;
				if (!End())
				{
					(*pIter)++;
				}

				if (!End())
				{
					v.k = (*pIter)->first;
					v.v = (*pIter)->second;
				}
			}
			else if (m_pIter != NULL && m_iType == ArrayType)
			{
				list<InterfaceEx>::iterator* pIter = (list<InterfaceEx>::iterator*)m_pIter;
				if (!End())
				{
					(*pIter)++;
				}

				if (!End())
				{
					v.v = **pIter;
				}
			}

			return v;
		}

		bool InterfaceEx::End() const
		{
			bool bFlag = false;

			if (m_pIter != NULL && m_iType == ObjectType)
			{
				map<CString, InterfaceEx>::iterator* pIter = (map<CString, InterfaceEx>::iterator*)m_pIter;
				map<CString, InterfaceEx>* p = (map<CString, InterfaceEx>*)m_pData;

				if ((*pIter) == p->end())
				{
					bFlag = true;
					delete pIter;
					m_pIter = NULL;
				}
			}
			else if (m_pIter != NULL && m_iType == ArrayType)
			{
				list<InterfaceEx>* p = (list<InterfaceEx>*)m_pData;
				list<InterfaceEx>::iterator* pIter = (list<InterfaceEx>::iterator*)m_pIter;

				if ((*pIter) == p->end())
				{
					bFlag = true;
					delete pIter;
					m_pIter = NULL;
				}
			}
			if (m_pIter == NULL && (m_iType == ArrayType || m_iType == ObjectType))
			{
				bFlag = true;
			}

			return bFlag;
		}

		void InterfaceEx::PushBack(const InterfaceEx& v)	const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			p->push_back(v);
		}

		void InterfaceEx::PopBack() const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			p->pop_back();
		}

		void InterfaceEx::PushFront(const Ext::CPP::InterfaceEx &v) const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			p->push_front(v);
		}

		void InterfaceEx::PopFront() const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			p->pop_front();
		}

		void InterfaceEx::Delete(const int& i) const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			if (i < 0 || i >= (int)p->size())
			{
				return;
			}

			int iPos = 0;
			list<InterfaceEx>::iterator iter = p->begin();
			while (iter != p->end())
			{
				if (iPos == i)
				{
					p->erase(iter);
					return;
				}
				iPos++;
				iter++;
			}
		}

		void InterfaceEx::Insert(const int& i, const InterfaceEx& v) const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			if (i < 0 || i >= (int)p->size())
			{
				return;
			}

			int iPos = 0;
			list<InterfaceEx>::iterator iter = p->begin();
			while (iter != p->end())
			{
				if (iPos == i)
				{
					p->insert(iter, v);
					return;
				}
				iPos++;
				iter++;
			}
		}

		InterfaceEx& InterfaceEx::At(const int& i) const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			if (i < 0 || i >= (int)p->size())
			{
				return InterfaceEx();	// ????????????
			}

			int iPos = 0;
			list<InterfaceEx>::iterator iter = p->begin();
			while (iter != p->end())
			{
				if (iPos == i)
				{
					return *iter;
				}
				iPos++;
				iter++;
			}

			return InterfaceEx();		// ????????????
		}

		InterfaceEx& InterfaceEx::operator [](const int& i) const
		{
			return At(i);
		}

		bool InterfaceEx::Empty() const
		{
			bool bFlag = false;
			if (m_iType == ObjectType && m_pData != NULL)
			{
				map<CString, InterfaceEx>* p = (map<CString, InterfaceEx>*)m_pData;
				bFlag = p->empty();
			}
			else if (m_iType == ArrayType && m_pData != NULL)
			{
				list<InterfaceEx>* p = (list<InterfaceEx>*)m_pData;
				bFlag = p->empty();
			}
			return bFlag;
		}

		void InterfaceEx::RemoveAll() const
		{
			if (m_iType == ObjectType && m_pData != NULL)
			{
				map<CString, InterfaceEx>* p = (map<CString, InterfaceEx>*)m_pData;
				p->clear();
			}
			else if (m_iType == ArrayType && m_pData != NULL)
			{
				list<InterfaceEx>* p = (list<InterfaceEx>*)m_pData;
				p->clear();
			}
		}

		int InterfaceEx::Count() const
		{
			int iCount = 0;

			if (m_iType == ObjectType && m_pData != NULL)
			{
				map<CString, InterfaceEx>* p = (map<CString, InterfaceEx>*)m_pData;
				iCount = (int)p->size();
			}
			else if (m_iType == ArrayType && m_pData != NULL)
			{
				list<InterfaceEx>* p = (list<InterfaceEx>*)m_pData;
				iCount = (int)p->size();
			}

			return iCount;
		}

		void InterfaceEx::Update(const int& i, const InterfaceEx& v) const
		{
			list<InterfaceEx>* p = NULL;
			if (m_iType != ArrayType || m_pData == NULL)
			{
				Clear();

				p = new list<InterfaceEx>;
				m_pData = p;
				m_iType = ArrayType;
			}

			p = (list<InterfaceEx>*)m_pData;
			if (i < 0 || i >= (int)p->size())
			{
				return;
			}

			int iPos = 0;
			list<InterfaceEx>::iterator iter = p->begin();
			while (iter != p->end())
			{
				if (iPos == i)
				{
					*iter = v;
					return;
				}
				iPos++;
			}
		}

		InterfaceEx::ValueType InterfaceEx::GetType() const
		{
			return this->m_iType;
		}

		bool InterfaceEx::IsNullType()
		{
			return (m_iType == InterfaceEx::NullType);
		}

		bool InterfaceEx::IsNumeric()
		{
			if (m_iType == InterfaceEx::IntType)
			{
				return true;
			}
			else if (m_iType == InterfaceEx::LongType)
			{
				return true;
			}
			else if (m_iType == InterfaceEx::DoubleType)
			{
				return true;
			}
			return false;
		}

		void InterfaceEx::ClearBool() const
		{
			if (m_pData != NULL && m_iType == BoolType)
			{
				delete ((bool*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearInt() const
		{
			if (m_pData != NULL && m_iType == IntType)
			{
				delete ((int*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearUint64() const
		{
			if (m_pData != NULL && m_iType == Uint64Type)
			{
				delete ((UINT64*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearLong() const
		{
			if (m_pData != NULL && m_iType == LongType)
			{
				delete ((long*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearDouble() const
		{
			if (m_pData != NULL && m_iType == DoubleType)
			{
				delete ((double*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearString() const
		{
			if (m_pData != NULL && m_iType == StringType)
			{
				delete ((CString*)m_pData);
				m_pData = NULL;
			}
		}

		void InterfaceEx::ClearArray() const
		{
			if (m_pData != NULL && m_iType == ArrayType)
			{
				delete ((std::list<InterfaceEx>*)m_pData);
				m_pData = NULL;		
			}
		}

		void InterfaceEx::ClearObject() const
		{
			if (m_pData != NULL && m_iType == ObjectType)
			{
				delete ((map<CString, InterfaceEx>*)m_pData);;
				m_pData = NULL;
			}
		}

		void InterfaceEx::Clear() const
		{
			ClearBool();
			ClearInt();
			ClearUint64();
			ClearLong();
			ClearDouble();
			ClearString();
			ClearArray();
			ClearObject();

			if (m_pIter != NULL)
			{
				delete m_pIter;
				m_pIter = NULL;
			}
		}

	} // End namespace CPP

} // End namesapce Ext