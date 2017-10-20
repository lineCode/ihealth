#include "StdAfx.h"
#include "MdbLayerCodeDefine.h"
#include "Tool.h"
#include "SettingDatabase.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbLayerCodeDefine::MdbLayerCodeDefineTableName = _T("LayerCodeDefine");
const CString CMdbLayerCodeDefine::ID = _T("ID");

CMdbLayerCodeDefine::CMdbLayerCodeDefine(void)
{
	this->m_Name = CMdbLayerCodeDefine::MdbLayerCodeDefineTableName;
}

CMdbLayerCodeDefine::~CMdbLayerCodeDefine(void)
{
}

int CMdbLayerCodeDefine::OnReflectTo()
{
	if (this->m_Fields.IsEmpty())
		return -1;

	CAdoField *pField = this->GetField(CMdbLayerCodeDefine::ID);
	pField->SetAsAutoIncreaseField();
	pField->SetAsPrimaryKey();

	return 0;
}

CMdbLayerCodeDefine& CMdbLayerCodeDefine::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbLayerCodeDefine::MdbLayerCodeDefineTableName) == false)
	{
		CMdbLayerCodeDefine *pParams = new CMdbLayerCodeDefine();
		setting.AddTable(pParams);
	}
	
	CAdoTable *pTable = setting.GetTable(CMdbLayerCodeDefine::MdbLayerCodeDefineTableName);
	CMdbLayerCodeDefine *pParams = static_cast<CMdbLayerCodeDefine*>(pTable);
	return *pParams;
}

bool CMdbLayerCodeDefine::GetByProject(List<CAdoRow*> &output, long projectId)
{
	bool flag = this->Query(output, _T("select [layerName], [propName], [propVal], [code] from $ where [prjId]=%d"), projectId);
	return flag;
}

bool CMdbLayerCodeDefine::SGetByProject(List<CAdoRow*> &output, long projectId)
{
	return CMdbLayerCodeDefine::Instance().GetByProject(output, projectId);
}

bool CMdbLayerCodeDefine::GetByProjectAndLayer(List<CAdoRow*> &output, long projectId, const CString &layerName)
{
	bool flag = this->Query(output, _T("select [propName], [propVal], [code] from $ where [prjId]=%d and [layerName]='%s'"), projectId, layerName);
	return flag;
}

bool CMdbLayerCodeDefine::SGetByProjectAndLayer(List<CAdoRow*> &output, long projectId, const CString &layerName)
{
	return CMdbLayerCodeDefine::Instance().GetByProjectAndLayer(output, projectId, layerName);
}

bool CMdbLayerCodeDefine::GetByProjectAndPropVal(CString &outCode, long projectId, const CString &propVal)
{
	CAdoRow *pRow = NULL;
	bool flag = this->Query(pRow, _T("select [code] from $ where [prjId]=%d and [propVal]='%s'"), projectId, propVal);
	if (pRow != NULL)
	{
		outCode = pRow->GetField(0).GetStringValue();
		delete pRow;
		pRow = NULL;
	}
	return flag;
}

bool CMdbLayerCodeDefine::SGetByProjectAndPropVal(CString &outCode, long projectId, const CString &propVal)
{
	return CMdbLayerCodeDefine::Instance().GetByProjectAndPropVal(outCode, projectId, propVal);
}

bool CMdbLayerCodeDefine::GetByCode(CAdoRow *&pOutRow, const CString &code, const CString &layernmae)
{
	bool flag = this->Query(pOutRow, _T("select * from $ where [code]='%s' and [layerName]='%s'"), code, layernmae);
	return flag;
}

bool CMdbLayerCodeDefine::SGetByCode(CAdoRow *&pOutRow, const CString &code, const CString &layernmae)
{
	return CMdbLayerCodeDefine::Instance().GetByCode(pOutRow, code, layernmae);
}
