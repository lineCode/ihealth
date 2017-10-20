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
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("空值检测"), projectID);
}

bool CMdbCheckedOption::SGetCheckNullOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckNullOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckRepeatOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("重复性"), projectID);
}

bool CMdbCheckedOption::SGetCheckRepeatOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckRepeatOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckClosedOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("闭合性"), projectID);
}

bool CMdbCheckedOption::SGetCheckClosedOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckClosedOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckZhuhaiRelationOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("许可证证号、收件编号、许可证核发时间关联性"), projectID);
}

bool CMdbCheckedOption::SGetCheckZhuhaiRelationOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckZhuhaiRelationOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckRegexpOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("属性值规范性检测"), projectID);
}

bool CMdbCheckedOption::SGetCheckRegexpOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckRegexpOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckAttachmentOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("附件检测"), projectID); 
}

bool CMdbCheckedOption::SGetCheckAttachmentOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckAttachmentOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckFixLengthOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("固定长度"), projectID); 
}

bool CMdbCheckedOption::SGetCheckFixLengthOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckFixLengthOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckYDDaimaAndXingzhiOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("用地代码与用地性质关联性"), projectID); 
}

bool CMdbCheckedOption::SGetCheckYDDaimaAndXingzhiOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckYDDaimaAndXingzhiOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetRemoveEmbedLineOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("删除完全重叠多段线"), projectID); 
}

bool CMdbCheckedOption::SGetRemoveEmbedLineOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetRemoveEmbedLineOptions(outOptions, projectID);
}

bool CMdbCheckedOption::GetCheckPipeLineMatchOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return this->Query(outOptions, _T("select * from $ where CheckType='%s' AND ProjectID=%d;"), _T("管线管点关联性检测"), projectID); 
}

bool CMdbCheckedOption::SGetCheckPipeLineMatchOptions(List<CAdoRow*> &outOptions, long projectID)
{
	return CMdbCheckedOption::Instance().GetCheckPipeLineMatchOptions(outOptions, projectID);
}
