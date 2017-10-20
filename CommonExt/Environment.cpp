#include "StdAfx.h"
#include "Environment.h"
#include "FilePath.h"
#include "IniFile.h"
//#include <intrin.h>

#include "..\inc\Definition.h"

using namespace Ext;

CEnvironment::CEnvironment(void)
{
}

CEnvironment::~CEnvironment(void)
{
}

CString CEnvironment::GetWorkingDirectory()
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	HMODULE hCallerModule = NULL;
	//void *callerAddress = _ReturnAddress();   
	//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)callerAddress, &hCallerModule);
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, _T("DCICommonExt"), &hCallerModule);

	::GetModuleFileName(hCallerModule, fullPath, FILENAME_MAX-1);

	TCHAR drive[_MAX_PATH];
	drive[0]='\0';

	TCHAR directory[_MAX_PATH];
	directory[0]='\0';

	CString rs;

	_tsplitpath(fullPath, drive, directory, NULL, NULL);
	rs.Format(_T("%s%s"), drive, directory);

	return rs;
}


CString Ext::CEnvironment::GetProjectDirectory()
{
	CString path = CEnvironment::GetWorkingDirectory();
	CString file = CFilePath::Combine(CEnvironment::GetWorkingDirectory(), _T("PrjParam"));

	//项目配置文件不存在，直接返回当前的工作目录
	if (CFilePath::IsFileExist(file) == FALSE)
		return path;

	//配置文件
	CString strParam;
	strParam = Ext::File::CIniFile::ReadKey(_T("配置文件"),_T("路径"), strParam, file);
	strParam = strParam.Trim();
	if (strParam.GetLength() < 1)
		return path;

	//项目配置目录
	CString param = CFilePath::Combine(CEnvironment::GetWorkingDirectory(), strParam);
	if (CFilePath::IsFileExist(param) == FALSE)
		return path;
	else
		return param;
}


CString Ext::CEnvironment::GetCommonExtModulePath()
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	HMODULE hCallerModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, _T("DCICommonExt"), &hCallerModule);

	::GetModuleFileName(hCallerModule, fullPath, FILENAME_MAX-1);
	CString rs = fullPath;

	return rs;
}

CString CEnvironment::GetStartFullPath()
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	//::GetModuleFileName(_hdllInstance, fullPath, FILENAME_MAX-1);
	::GetModuleFileName(NULL, fullPath, FILENAME_MAX-1);

	CString rs = fullPath;
	return rs;
}

CString CEnvironment::GetStartFileName(bool withExtentionname/*=true*/)
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	::GetModuleFileName(NULL, fullPath, FILENAME_MAX-1);

	TCHAR fileName[FILENAME_MAX];
	fileName[0]='\0';

	CString rs;

	if (withExtentionname)
	{
		TCHAR extionName[FILENAME_MAX];
		extionName[0]='\0';

		_tsplitpath(fullPath, NULL, NULL, fileName, extionName);
		rs.Format(_T("%s%s"), fileName, extionName);
	}
	else
	{
		_tsplitpath(fullPath, NULL, NULL, fileName, NULL);
		rs = fileName;
	}
	
	return rs;
}

CString CEnvironment::GetStartFileExtionName()
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	::GetModuleFileName(NULL, fullPath, FILENAME_MAX-1);

	TCHAR extionName[FILENAME_MAX];
	extionName[0]='\0';

	_tsplitpath(fullPath, NULL, NULL, NULL, extionName);

	CString rs = extionName;

	return rs;
}

CString CEnvironment::GetWorkingDrive()
{
	TCHAR fullPath[FILENAME_MAX];
	fullPath[0]='\0';

	::GetModuleFileName(NULL, fullPath, FILENAME_MAX-1);

	TCHAR drive[_MAX_PATH];
	drive[0]='\0';

	_tsplitpath(fullPath, drive, NULL, NULL, NULL);

	CString rs = drive;

	return rs;
}

CString CEnvironment::GetCurrentDateTime(LPCTSTR pFormat)
{
	CTime time = CTime::GetCurrentTime();
	CString result = time.Format(_T("%Y%m%d_%H%M%S.log"));
	return result;
}

CString CEnvironment::GetProfileSetting(const CString &strAppName, const CString &strAppKey, const CString &strDefault/* = _T("")*/)
{
	CString dir = CEnvironment::GetProjectDirectory();//CEnvironment::GetWorkingDirectory();
	CString file = CFilePath::Combine(dir, _T("Settings.ini"));
	
	TCHAR szRightUrl[1024];
	memset(szRightUrl, 0, 1024);
	GetPrivateProfileString(strAppName, strAppKey, strDefault, szRightUrl, 1024, file);

	CString result = szRightUrl;
	return result;
}

CString CEnvironment::GetGisRightSvcURL()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, PROFILE_GIS_RIGHT_URL);
}

CString CEnvironment::GetCaseAttachUploadURL()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, PROFILE_CASE_UPLOAD_URL);
}

CString CEnvironment::GetCaseDataSvcURL()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, PROFILE_CASE_DATASVC_URL);
}

CString CEnvironment::GetCheckDataSvcURL()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, PROFILE_CHECK_DATASVC_URL);
}

CString Ext::CEnvironment::GetUserSource()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, PROFILE_USER_FROM_OA_OR_GIS);
}

CString Ext::CEnvironment::GetGLTZServiceURL()
{
	return CEnvironment::GetProfileSetting(PROFILE_CASE_PARAM, _T("GLTZ_URL"));
}
