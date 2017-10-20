#include "StdAfx.h"
#include "MdbParams.h"
#include "Tool.h"
#include "SettingDatabase.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbParams::MdbParamsTableName = _T("params");
const CString CMdbParams::ID = _T("ID");

CMdbParams::CMdbParams(void)
{
	this->m_Name = CMdbParams::MdbParamsTableName;
}

CMdbParams::~CMdbParams(void)
{
}

int CMdbParams::OnReflectTo()
{
	if (this->m_Fields.IsEmpty())
		return -1;

	CAdoField *pField = this->GetField(CMdbParams::ID);
	pField->SetAsAutoIncreaseField();
	pField->SetAsPrimaryKey();

	return 0;
}

CMdbParams& CMdbParams::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbParams::MdbParamsTableName) == false)
	{
		CMdbParams *pParams = new CMdbParams();
		setting.AddTable(pParams);
	}
	
	CAdoTable *pTable = setting.GetTable(CMdbParams::MdbParamsTableName);
	CMdbParams *pParams = static_cast<CMdbParams*>(pTable);
	return *pParams;
}

CString CMdbParams::GetValue(const CString &strParamName)
{
	CString result;
	this->GetValue(result, strParamName);

	return result;
}

bool CMdbParams::GetValue(CString &output, const CString &strParamName)
{
	List<CAdoRow*> rows;
	bool flag = this->Query(rows, _T("select val from $ where [name]='%s'"), strParamName);

	if (rows.IsEmpty())
	{
		//output = _T("");
		return flag;
	}

	output = (*rows[0])[0].GetStringValue();
	CTool::ReleaseList(rows);

	return flag;
}

bool CMdbParams::SetValue(const CString &strParamName, const CString &strParamValue)
{
	//CAdoRow *pRow = NULL;
	//bool flag = this->Query(pRow, _T("select id, val from $ where [name]='%s'"), strParamName);
	//if (pRow == NULL || flag == false)
	//	return false;

	//CAdoRow &row = *pRow;
	//row[_T("val")] = strParamValue;
	//flag = this->ModifyRow(row);
	//delete pRow;
	//pRow = NULL;
	//return flag;

	return this->ModifyRows(_T("set val='%s' where name='%s';"), strParamValue, strParamName);
}

CString CMdbParams::SGetValue(const CString &strParamName)
{
	return CMdbParams::Instance().GetValue(strParamName);
}

bool CMdbParams::SGetValue(CString &output, const CString &strParamName)
{
	return CMdbParams::Instance().GetValue(output, strParamName);
}

bool CMdbParams::SSetValue(const CString &strParamName, const CString &strParamValue)
{
	return CMdbParams::Instance().SetValue(strParamName, strParamValue);
}