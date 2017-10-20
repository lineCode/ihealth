#include "stdafx.h"
#include "CSMModule.h"
#include "Environment.h"
#include "FilePath.h"
#include "ExString.h"

#include <fstream>

using namespace Ext;

CSMModule* CSMModule::DefaultModule = NULL;

CSMModule::CSMModule()
{
	Name = _T("");
	Client = new RPCClient();
	Server = new RPCServer();
}

Ext::CSMModule::CSMModule(RPCClient *client, RPCServer *server)
{
	Name = _T("");
	Client = client;
	Server = server;
}

CSMModule::~CSMModule()
{
	if (Client != RPCClient::DefaultClient)
		delete Client;

	if (Server != RPCServer::DefaultServer)
		delete Server;
}

Ext::CPP::Panic CSMModule::InsSetName(const CString &moduleName)
{
	Name = moduleName;
	return Panic();
}

Ext::CPP::Panic CSMModule::InsMakesureCSMReady()
{
	Panic panic(0, _T("OK"));
	HANDLE csmFlag = ::OpenEvent(EVENT_ALL_ACCESS, false, _T("EvetnForCSMFlag"));

	// CSM 已经存在，只需确保 CSM 是否准备就绪
	if (csmFlag != NULL)
	{
		HANDLE csmReady = ::OpenEvent(EVENT_ALL_ACCESS, false, _T("EventOnCSMReady"));
		if (::WaitForSingleObject(csmReady, INFINITE) == WAIT_OBJECT_0)
		{
			::SetEvent(csmReady);
			return panic;
		}
	}

	// CSM 不存在，需要显示地启动 CSM，并等待其准备就绪
	STARTUPINFO si; 
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_SHOW;
	//si.wShowWindow = SW_HIDE;

	CString workingFolder = CEnvironment::GetWorkingDirectory();
	CString	processExe = CFilePath::Combine(workingFolder, _T("CSM.exe"));

	CFileFind ff;
	if (!ff.FindFile(processExe))
	{
		workingFolder = CFilePath::Combine(CEnvironment::GetWorkingDirectory(), _T("ToolBoxBin"));
		processExe = CFilePath::Combine(workingFolder, _T("CSM.exe"));
	}
	ff.Close();

	CString cmdLine = _T("");

	PROCESS_INFORMATION processInfo;
	BOOL ok = ::CreateProcess(processExe, cmdLine.GetBuffer(), NULL, NULL, false, 0, NULL, workingFolder, &si, &processInfo);
	if (!ok)
	{
		panic.Set(-1, _T("无法正常启动 CSM 服务"));
	}

	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);

	if (panic.OK() == true)
	{
		HANDLE csmReady = CreateEvent(NULL, false, false, _T("EventOnCSMReady"));
		if (::WaitForSingleObject(csmReady, INFINITE) == WAIT_OBJECT_0)
		{
			::SetEvent(csmReady);	// WaitForSingleObject 返回后，csmReady 被设置为无信号，需要显示设置有信号
			return panic;
		}
	}
	return panic;
}

Ext::CPP::Panic CSMModule::InsSetupRPC()
{
	Panic panic;

	///// 当前模块做为 RPC 服务端与 CSM 通讯
	char srvaddr[20]; u_short srvport = 0;
	::ZeroMemory(srvaddr, 20);
	
	panic = Server->InsListen(NULL, 0, srvaddr, &srvport);
	//panic = m_Server->InsListen("127.0.0.1", 27015, srvaddr, &srvport);
	if (panic.OK() == false)
		return panic;

	panic = Server->InsAccept();
	if (panic.OK() == false)
		return panic;

	///// 当前模块做为 RPC 客户端与 CSM 通讯
	char ip[20]; u_short port;
	::ZeroMemory(ip, 20);
	CSMModule::getRootServiceAddr(ip, &port);

	panic = Client->InsDial(ip, port);
	//panic = m_Client->InsDial("127.0.0.1", 10084);
	if (panic.OK() == false)
	{
		panic.Set(-1, _T("错误，附加功能将无法使用：%s\n"), panic.Info);
		return panic;
	}

	TCHAR *iptemp = CExString::char2TCHAR(srvaddr);
	CString srvip = iptemp;
	free(iptemp);

	JsonValue *args = JsonValue::New();
	args->Insert(_T("IP"), srvip);
	args->Insert(_T("Port"), srvport);
	args->Insert(_T("ModuleName"), Name);

	RPCResponse* pResp = Client->InsCall(_T("CSM/ModuleManager.RegistModule"), args);
	DEFER(pResp, Ext::RPCResponse::Release);
	CString reply = pResp->GetStringVal(_T("OK"));

	panic.Set(0, _T("CSM/ModuleManager.RegistModule response:%s\n"), reply);
	return panic;
}

Panic CSMModule::getRootServiceAddr(char *ip, u_short *port)
{
	CString filepath = CFilePath::Combine(CEnvironment::GetWorkingDirectory(), _T("RootServiceAddr.txt"));
	
	CFileFind ff;
	if (ff.FindFile(filepath) == FALSE)
	{
		filepath = CFilePath::Combine(CEnvironment::GetWorkingDirectory(), _T("ToolBoxBin\\RootServiceAddr.txt"));
	}
	ff.Close();

	std::ifstream inf(filepath);
	std::string line;

	char addr[20];
	inf.getline(addr, 19);
	inf.close();

	char *subs = NULL;
	subs = strtok(addr, ":");
	strcpy(ip, subs);

	subs = strtok( NULL, ":");
	char *endptr;
	unsigned long p = strtoul(subs, &endptr, 10);
	*port = (u_short)p;
	return Panic();
}

Ext::CPP::Panic Ext::CSMModule::InsTeardownRPC()
{
	Panic panic;

	RPCResponse* pResp = Client->Call(_T("CSM/ModuleManager.UnregistModule"), JsonValue::New(_T("ModuleName"), Name));
	DEFER(pResp, Ext::RPCResponse::Release);
	CString reply = pResp->GetStringVal(_T("OK"));

	panic.Set(0, _T("CSM/ModuleManager.UnregistModule response:%s\n"), reply);
	return panic;
}

Ext::CPP::Panic Ext::CSMModule::InsRelease()
{
	delete this;
	return Panic();
}

Ext::CPP::Panic Ext::CSMModule::SetName(const CString &moduleName)
{
	if (DefaultModule == NULL)
	{
		initDefaultRPC();
		DefaultModule = new CSMModule(RPCClient::DefaultClient, RPCServer::DefaultServer);
	}
	return DefaultModule->InsSetName(moduleName);
}

Ext::CPP::Panic Ext::CSMModule::MakesureCSMReady()
{
	if (DefaultModule == NULL)
	{
		initDefaultRPC();
		DefaultModule = new CSMModule(RPCClient::DefaultClient, RPCServer::DefaultServer);
	}
	return DefaultModule->InsMakesureCSMReady();
}

Ext::CPP::Panic Ext::CSMModule::SetupRPC()
{
	if (DefaultModule == NULL)
	{
		initDefaultRPC();
		DefaultModule = new CSMModule(RPCClient::DefaultClient, RPCServer::DefaultServer);
	}
	return DefaultModule->InsSetupRPC();
}

Ext::CPP::Panic Ext::CSMModule::TeardownRPC()
{
	if (DefaultModule == NULL)
		return Panic();
	return DefaultModule->InsTeardownRPC();
}

void Ext::CSMModule::initDefaultRPC()
{
	if (RPCClient::DefaultClient == NULL)
		RPCClient::DefaultClient = new RPCClient();

	if (RPCServer::DefaultServer == NULL)
		RPCServer::DefaultServer = new RPCServer();
}

Ext::CPP::Panic CSMModule::Release()
{
	Panic panic;
	if (DefaultModule != NULL)
	{
		panic = DefaultModule->InsRelease();
		DefaultModule = NULL;
	}
	return panic;
}