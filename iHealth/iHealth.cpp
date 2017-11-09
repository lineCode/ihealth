// iHealth.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "RFMainWindow.h"
#include "WkeWebkit.h"
#include "Log.h"

#include "robot_test.h"


long   __stdcall   ExcuteExceptionHandler(_EXCEPTION_POINTERS*   excp)   
{   
	return   EXCEPTION_EXECUTE_HANDLER;   
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	
	SetUnhandledExceptionFilter(ExcuteExceptionHandler);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CWkeWebkitUI::WkeWebkit_Init();

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	RFMainWindow* pFrame = new RFMainWindow();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("上肢外骨骼康复机器人"), UI_WNDSTYLE_FRAME, 0L, 0, 0, RF_WINDOW_WIDTH, RF_WINDOW_HEIGHT);
	pFrame->CenterWindow();
	pFrame->SetIcon(_T("MONDRIAN"));
	::ShowWindow(pFrame->GetHWND(), SW_NORMAL);
	//::SetWindowPos(pFrame->GetHWND(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	CPaintManagerUI::MessageLoop();

	CWkeWebkitUI::WkeWebkit_Shutdown();
	delete pFrame;
	pFrame = NULL;
	::CoUninitialize();

	//robot_test_start();
	return 0;
}