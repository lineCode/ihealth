#include "StdAfx.h"
#include "MdbCheckedOption.h"
#include "Tool.h"
#include "SettingDatabase.h"
#include "AdoView.h"

using namespace Ext;
using namespace Dci::SettingMDB;

const CString CMdbCheckedOption::MdbCheckedOptionTableName = _T("CheckedOption");

CMdbCheckedOption::CMdbCheckedOption(void)
{
	this->m_Name = CMdbCheckedOption::MdbCheckedOptionTableName;
}

CMdbCheckedOption::~CMdbCheckedOption(void)
{
}

int CMdbCheckedOption::OnReflectTo()
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

CMdbCheckedOption& CMdbCheckedOption::Instance()
{
	CSettingDatabase &setting = CSettingDatabase::Instance();
	if (setting.IsTableExist(CMdbCheckedOption::MdbCheckedOptionTableName) == false)
	{
		CMdbCheckedOption *pTable = new CMdbCheckedOption();
		setting.AddTable(pTable);
	}
	
	CAdoTable *pTable = setting.GetTable(CMdbCheckedOption::MdbCheckedOptionTableName);
	CMdbCheckedOption *pRealTable = static_cast<CMdbCheckedOption*>(pTable);
	return *pRealTable;
}

bool CMdbCheckedOption::GetCheckNullOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("��ֵ���"), projectID);
}

bool CMdbCheckedOption::SGetCheckNullOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckNullOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckRepeatOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("�ظ���"), projectID);
}

bool CMdbCheckedOption::SGetCheckRepeatOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckRepeatOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckClosedOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("�պ���"), projectID);
}

bool CMdbCheckedOption::SGetCheckClosedOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckClosedOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckZhuhaiRelationOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("���֤֤�š��ռ���š����֤�˷�ʱ�������"), projectID);
}

bool CMdbCheckedOption::SGetCheckZhuhaiRelationOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckZhuhaiRelationOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckRegexpOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("����ֵ�淶�Լ��"), projectID);
}

bool CMdbCheckedOption::SGetCheckRegexpOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckRegexpOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckAttachmentOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("�������"), projectID); 
}

bool CMdbCheckedOption::SGetCheckAttachmentOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckAttachmentOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckFixLengthOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("�̶�����"), projectID); 
}

bool CMdbCheckedOption::SGetCheckFixLengthOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckFixLengthOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckYDDaimaAndXingzhiOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("�õش������õ����ʹ�����"), projectID); 
}

bool CMdbCheckedOption::SGetCheckYDDaimaAndXingzhiOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckYDDaimaAndXingzhiOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetRemoveEmbedLineOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("ɾ����ȫ�ص������"), projectID); 
}

bool CMdbCheckedOption::SGetRemoveEmbedLineOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetRemoveEmbedLineOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckPipeLineMatchOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("���߹ܵ�����Լ��"), projectID); 
}

bool CMdbCheckedOption::SGetCheckPipeLineMatchOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckPipeLineMatchOptions(outOptions, projectID);
}
