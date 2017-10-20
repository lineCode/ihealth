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
	// ��������ɣ�
	// 1. ʹ�� strSdeLayer �� contrastLayer ���л��Ψһ��Ӧ�� ID �ֶε�ֵ id
	// 2. ʹ�� 1) �л�õ� id����Ӧ layConId�� ֵ�� strSpecialValue ֵ�� contrastProp ���л�ȡ SdeProp �ֶε�ֵ
	
	// �������� contrastLayer��contrastProp ��ʹ�ò�ѯһ�����

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

	// ���ص�ÿ����¼�а������У�sde���Ե����ơ����Ե��������͡����Եĳ���
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

	// ���ص�ÿ����¼�а������У�sde���Ե����ơ�special value ��ֵ
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

		// Ĭ���������ͺͳ���
		CString dataType = _T("�ַ�");
		long dataLenth = 100;

		CString specialVal = list[i]->GetField(_T("specialVal")).GetStringValue();
		if (specialVal != _T(""))
		{
			if (specialVal.CollateNoCase(_T("starttime")) == 0 || specialVal.CompareNoCase(_T("endtime")) == 0 || 
				specialVal.CollateNoCase(_T("����ʱ��")) == 0 || specialVal.CollateNoCase(_T("����ʱ��")) == 0 || 
				specialVal.CollateNoCase(_T("���ʱ��")) == 0)
			{
				dataType = _T("����");
				dataLenth = 30; // shp �ļ��ֶε�date��ȹ̶�Ϊ8
			}
		}

		if (specialVal != _T(""))
		{
			if (specialVal == _T("ͼ�����") || specialVal == _T("ͼ���ܳ�") || specialVal == _T("ͼ�κ��") ||
				specialVal == _T("���ִ�С") || specialVal == _T("���ֽǶ�") || specialVal == _T("���ֿ�߱�")  || specialVal == _T("������ת��"))
			{
				dataType = _T("ʵ��");
				dataLenth = 20;
			}
		}

		if (specialVal != _T(""))
		{
			if (specialVal == _T("����б��") || specialVal == _T("���ִ���") || specialVal == _T("�����»���") ||
				specialVal == _T("����ɾ����") || specialVal == _T("������ɫ"))
			{
				dataType = _T("����");
				dataLenth = 10;
			}
		}

		CAdoField &dataTypeField = pRow->GetField(_T("dataType"));
		CAdoField &lengthField = pRow->GetField(_T("length"));
		
		dataTypeField = dataType;
		lengthField = dataLenth;

		output.Append(pRow);
	}
	// ���ص�ÿ����¼�а������У�sde���Ե����ơ����Ե��������͡����Եĳ���
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

	// ���ص�ÿ����¼�а������У�sde���Ե����ơ�cad���Ե����ơ�����ֵ
	bool flag = pView->Query(output, sql, strSdeLayer, projectID);
	delete pView;

	return flag;
}

bool CMdbContrastProp::SGetContrastCadInfoForSdeLayer(List<CAdoRow*> &output, long projectID, const CString &strSdeLayer)
{
	return CMdbContrastProp::Instance().GetContrastCadInfoForSdeLayer(output, projectID, strSdeLayer);
}