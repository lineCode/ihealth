#include "StdAfx.h"
#include "MdbContrastLayer.h"
#include "Tool.h"
#include "SettingDatabase.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbContrastLayer::MdbContrastLayerTableName = _T("contrastLayer");

CMdbContrastLayer::CMdbContrastLayer(void)
{
	this->m_Name = CMdbContrastLayer::MdbContrastLayerTableName;
}

CMdbContrastLayer::~CMdbContrastLayer(void)
{
}

int CMdbContrastLayer::OnReflectTo()
{
	if (this->m_Fields.IsEmpty())
		return -1;

	CAdoField *pField = this->GetField(_T("ID"));
	if (pField != NULL)
	{
		pField->SetAsAutoIncreaseField();
		pField->SetAsPrimaryKey();
	}
	
	return 0;
}

CMdbContrastLayer& CMdbContrastLayer::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbContrastLayer::MdbContrastLayerTableName) == false)
	{
		CMdbContrastLayer *pTable = new CMdbContrastLayer();
		setting.AddTable(pTable);
	}
	
	CAdoTable *pTable = setting.GetTable(CMdbContrastLayer::MdbContrastLayerTableName);
	CMdbContrastLayer *pRealTable = static_cast<CMdbContrastLayer*>(pTable);
	return *pRealTable;
}

bool CMdbContrastLayer::GetAllSdeLayers(List<CAdoRow*> &output, long projectId)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT distinct contrastLayer.sdeLayer, objectType.entityType ");
	sql += _T("FROM (contrastLayer INNER JOIN objectType ON contrastLayer.layerId = objectType.ID) INNER JOIN projectType ON objectType.prjId = projectType.ID ");
	sql += _T("WHERE (((projectType.ID)=%d));");

	bool flag = pView->Query(output, sql, projectId);
	delete pView;

	return flag;
}

bool CMdbContrastLayer::SGetAllSdeLayers(List<CAdoRow*> &output, long projectId)
{
	return CMdbContrastLayer::Instance().GetAllSdeLayers(output, projectId);
}

bool CMdbContrastLayer::GetContrastCadInfoForSdeLayer(List<CAdoRow*> &output, long projectId, const CString &strSdeLayer)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT objectType.layerName, contrastLayer.propName, contrastLayer.propVal ");
	sql += _T("FROM contrastLayer INNER JOIN (objectType INNER JOIN projectType ON objectType.prjId = projectType.ID) ON contrastLayer.layerId = objectType.ID ");
	sql += _T("WHERE (((projectType.ID)=%d) AND ((contrastLayer.sdeLayer)='%s'));");

	bool flag = pView->Query(output, sql, projectId, strSdeLayer);
	delete pView;

	return flag;
}

bool CMdbContrastLayer::SGetContrastCadInfoForSdeLayer(List<CAdoRow*> &output, long projectId, const CString &strSdeLayer)
{
	return CMdbContrastLayer::Instance().GetContrastCadInfoForSdeLayer(output, projectId, strSdeLayer);
}

bool CMdbContrastLayer::GetProjectIdBySdeLayer(long &outputProjectId, const CString &strSdeLayer)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT contrastLayer.sdeLayer, objectType.prjId ");
	sql += _T("FROM contrastLayer INNER JOIN objectType ON contrastLayer.layerId = objectType.ID ");
	sql += _T("WHERE (((contrastLayer.sdeLayer)='%s'));");

	CAdoRow *pRow = NULL;
	bool flag = pView->Query(pRow, sql, strSdeLayer);
	delete pView;

	if (pRow != NULL)
	{
		outputProjectId = pRow->GetField(_T("prjId")).GetLongValue();
		delete pRow;
	}

	return flag;
}

bool CMdbContrastLayer::SGetGetProjectIdBySdeLayer(long &outputProjectId, const CString &strSdeLayer)
{
	return CMdbContrastLayer::Instance().GetProjectIdBySdeLayer(outputProjectId, strSdeLayer);
}
