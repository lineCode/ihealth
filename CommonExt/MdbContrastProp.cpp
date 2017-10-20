#include "StdAfx.h"
#include "MdbContrastProp.h"
#include "Tool.h"
#include "SettingDatabase.h"
#include "AdoView.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbContrastProp::MdbContrastPropTableName = _T("contrastProp");

CMdbContrastProp::CMdbContrastProp(void)
{
	this->m_Name = CMdbContrastProp::MdbContrastPropTableName;
}

CMdbContrastProp::~CMdbContrastProp(void)
{
}

int CMdbContrastProp::OnReflectTo()
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

CMdbContrastProp& CMdbContrastProp::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbContrastProp::MdbContrastPropTableName) == false)
	{
		CMdbContrastProp *pTable = new CMdbContrastProp();
		setting.AddTable(pTable);
	}
	
	CAdoTable *pTable = setting.GetTable(CMdbContrastProp::MdbContrastPropTableName);
	CMdbContrastProp *pRealTable = static_cast<CMdbContrastProp*>(pTable);
	return *pRealTable;
}

bool CMdbContrastProp::GetSdePropertyBySpecialValue(CString &output, const CString &strSdeLayer, const CString &strSpecialValue)
{
	// 分两步完成：
	// 1. 使用 strSdeLayer 在 contrastLayer 表中获得唯一对应的 ID 字段的值 id
	// 2. 使用 1) 中获得的 id（对应 layConId） 值和 strSpecialValue 值在 contrastProp 表中获取 SdeProp 字段的值
	
	// 下面联合 contrastLayer、contrastProp 表，使用查询一步完成

	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT contrastProp.sdeProp ");
	sql += _T("FROM contrastLayer INNER JOIN contrastProp ON contrastLayer.ID = contrastProp.layConId ");
	sql += _T("WHERE (((contrastLayer.sdeLayer)='%s') AND ((contrastProp.specialVal)='%s'));");

	CAdoRow *pRow = NULL;
	bool flag = pView->Query(pRow, sql, strSdeLayer, strSpecialValue);
	delete pView;
	pView = NULL;

	if (flag == true && pRow != NULL)
	{
		output = pRow->GetField(0).GetStringValue();
		delete pRow;
		pRow = NULL;
		return true;
	}

	return false;
}

bool CMdbContrastProp::SGetSdePropertyBySpecialValue(CString &output, const CString &strSdeLayer, const CString &strSpecialValue)
{
	return CMdbContrastProp::Instance().GetSdePropertyBySpecialValue(output, strSdeLayer, strSpecialValue);
}

bool CMdbContrastProp::GetAllSdeProperty(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT distinct contrastProp.sdeProp, propTable.dataType, propTable.length ");
	sql += _T("FROM (objectType INNER JOIN (contrastProp INNER JOIN (contrastLayer INNER JOIN propTable ON contrastLayer.layerId = propTable.typeId) ON (contrastProp.layConId = contrastLayer.ID) AND (contrastProp.cadProp = propTable.name)) ON objectType.ID = propTable.typeId) INNER JOIN projectType ON objectType.prjId = projectType.ID ");
	sql += _T("WHERE (((contrastLayer.sdeLayer)='%s') AND ((projectType.ID)=%d));");

	// 返回的每条记录中包含三列：sde属性的名称、属性的数据类型、属性的长度
	bool flag = pView->Query(output, sql, strSdeLayer, projectID);
	delete pView;

	return flag;
}

bool CMdbContrastProp::SGetAllSdeProperty(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	return CMdbContrastProp::Instance().GetAllSdeProperty(output, projectID, strSdeLayer);
}

bool CMdbContrastProp::GetAllSpecialSdeProperty(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT distinct contrastProp.sdeProp, contrastProp.specialVal ");
	sql += _T("FROM projectType INNER JOIN (objectType INNER JOIN (contrastProp INNER JOIN contrastLayer ON contrastProp.layConId = contrastLayer.ID) ON objectType.ID = contrastLayer.layerId) ON projectType.ID = objectType.prjId ");
	sql += _T("WHERE (((contrastProp.cadProp) Is Null) AND ((contrastProp.specialVal)<>'') AND ((contrastLayer.sdeLayer)='%s') AND ((projectType.ID)=%d));");

	// 返回的每条记录中包含两列：sde属性的名称、special value 的值
	List<CAdoRow*> list;
	bool flag = pView->Query(list, sql, strSdeLayer, projectID);
	if (flag == false)
		return false;

	CAdoTable tempTable;
	CAdoField fieldSdeProp(CAdoField::Text, _T("sdeProp"));
	CAdoField fieldLength(CAdoField::Long, _T("length"));
	CAdoField fieldDataType(CAdoField::Text, _T("dataType"));
	tempTable.AddField(fieldSdeProp);
	tempTable.AddField(fieldLength);
	tempTable.AddField(fieldDataType);

	for (int i = 0; i < list.Count(); ++i)
	{
		CAdoRow *pRow = tempTable.NewRowAsPointer();
		CAdoField &sdeProp = pRow->GetField(_T("sdeProp"));
		sdeProp = list[i]->GetField(_T("sdeProp")).GetStringValue();

		// 默认数据类型和长度
		CString dataType = _T("字符");
		long dataLenth = 100;

		CString specialVal = list[i]->GetField(_T("specialVal")).GetStringValue();
		if (specialVal != _T(""))
		{
			if (specialVal.CollateNoCase(_T("starttime")) == 0 || specialVal.CompareNoCase(_T("endtime")) == 0 || 
				specialVal.CollateNoCase(_T("编制时间")) == 0 || specialVal.CollateNoCase(_T("审批时间")) == 0 || 
				specialVal.CollateNoCase(_T("入库时间")) == 0)
			{
				dataType = _T("日期");
				dataLenth = 30; // shp 文件字段的date宽度固定为8
			}
		}

		if (specialVal != _T(""))
		{
			if (specialVal == _T("图形面积") || specialVal == _T("图形周长") || specialVal == _T("图形厚度") ||
				specialVal == _T("文字大小") || specialVal == _T("文字角度") || specialVal == _T("文字宽高比")  || specialVal == _T("文字旋转角"))
			{
				dataType = _T("实数");
				dataLenth = 20;
			}
		}

		if (specialVal != _T(""))
		{
			if (specialVal == _T("文字斜体") || specialVal == _T("文字粗体") || specialVal == _T("文字下划线") ||
				specialVal == _T("文字删除线") || specialVal == _T("文字颜色"))
			{
				dataType = _T("整数");
				dataLenth = 10;
			}
		}

		CAdoField &dataTypeField = pRow->GetField(_T("dataType"));
		CAdoField &lengthField = pRow->GetField(_T("length"));
		
		dataTypeField = dataType;
		lengthField = dataLenth;

		output.Append(pRow);
	}
	// 返回的每条记录中包含三列：sde属性的名称、属性的数据类型、属性的长度
	delete pView;
	CTool::ReleaseList(list);

	return flag;
}

bool CMdbContrastProp::SGetAllSpecialSdeProperty(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	return CMdbContrastProp::Instance().GetAllSpecialSdeProperty(output, projectID, strSdeLayer);
}

bool CMdbContrastProp::GetContrastCadInfoForSdeLayer(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	CAdoView *pView = CSettingDatabase::Instance().CreateView();

	CString sql = _T("");
	sql += _T("SELECT contrastProp.sdeProp, contrastProp.cadProp, contrastProp.specialVal ");
	sql += _T("FROM (projectType INNER JOIN objectType ON projectType.ID = objectType.prjId) INNER JOIN (contrastProp INNER JOIN contrastLayer ON contrastProp.layConId = contrastLayer.ID) ON objectType.ID = contrastLayer.layerId ");
	sql += _T("WHERE (((contrastLayer.sdeLayer)='%s') AND ((projectType.ID)=%d));");

	// 返回的每条记录中包含三列：sde属性的名称、cad属性的名称、特殊值
	bool flag = pView->Query(output, sql, strSdeLayer, projectID);
	delete pView;

	return flag;
}

bool CMdbContrastProp::SGetContrastCadInfoForSdeLayer(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	return CMdbContrastProp::Instance().GetContrastCadInfoForSdeLayer(output, projectID, strSdeLayer);
}