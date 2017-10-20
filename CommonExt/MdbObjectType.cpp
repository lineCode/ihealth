#include "StdAfx.h"
#include "MdbObjectType.h"
#include "Tool.h"
#include "SettingDatabase.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbObjectType::MdbObjectTypeTableName = _T("objectType");
const CString CMdbObjectType::ID = _T("ID");
const CString CMdbObjectType::LayerName = _T("layerName");

CMdbObjectType::CMdbObjectType(void)
{
	this->m_Name = CMdbObjectType::MdbObjectTypeTableName;
}

CMdbObjectType::~CMdbObjectType(void)
{
}

int CMdbObjectType::OnReflectTo()
{
	if (this->m_Fields.IsEmpty())
		return -1;

	CAdoField *pField = this->GetField(CMdbObjectType::ID);
	pField->SetAsAutoIncreaseField();

	return 0;
}

CMdbObjectType& CMdbObjectType::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbObjectType::MdbObjectTypeTableName) == false)
	{
		CMdbObjectType *pObjectType = new CMdbObjectType();
		setting.AddTable(pObjectType);
	}

	CAdoTable *pTable = setting.GetTable(CMdbObjectType::MdbObjectTypeTableName);
	CMdbObjectType *pObjectType = static_cast<CMdbObjectType*>(pTable);
	return *pObjectType;
}

void CMdbObjectType::GetProjectLayers(List<CAdoRow*> &rows, const int& projectID)
{
	this->Query(rows, _T("select * from $ where %s=%d"), _T("prjId"), projectID);
}

void CMdbObjectType::GetProjectLayers(List<CString> &layerNames, const int& projectID)
{
	List<CAdoRow*> rows;
	this->Query(rows, _T("select [layerName] from $ where %s=%d"), _T("prjId"), projectID);

	for (int i = 0; i < rows.Count(); ++i)
	{
		layerNames.Append(rows[i]->GetField(0).GetStringValue());
	}

	CTool::ReleaseList(rows);
}

void CMdbObjectType::SGetProjectLayers(List<CAdoRow*> &rows, const int& projectID)
{
	CMdbObjectType::Instance().GetProjectLayers(rows, projectID);
}

void CMdbObjectType::SGetProjectLayers(List<CString> &layerNames, const int& projectID)
{
	CMdbObjectType::Instance().GetProjectLayers(layerNames, projectID);
}

CAdoRow* CMdbObjectType::GetOneRow(const int& projectID, const CString &layerName)
{
	List<CAdoRow*> rows;
	this->Query(rows, _T("select [ID],[prjId],[layerName],[entityType],[color],[linetype] from $ where %s=%d and %s='%s'"), _T("prjId"), projectID, _T("layerName"), layerName);

	if (rows.Count() == 0)
		return NULL;

	CAdoRow *first = rows[0];
	rows[0] = NULL;
	CTool::ReleaseList(rows);
	return first;
}

CAdoRow* CMdbObjectType::SGetOneRow(const int& projectID, const CString &layerName)
{
	return CMdbObjectType::Instance().GetOneRow(projectID, layerName);
}
