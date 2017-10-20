#pragma once
#include "ThreadBase.h"

class CUIThread : public CThreadBase
{
private:
	HWND m_Hwnd;

private:
	CUIThread(HWND hwnd);
	~CUIThread();

public:
	static Panic Create(HWND hwnd);
	static Panic Release(CUIThread *&pThread);

	virtual TaskNumber AssignTask(CTask *pTask);
	
	/// <summary>
	/// 不允许 UI 线程等待其他应答任务，否则界面将陷入假死状态
	/// </summary>
	/// <author name="hps" date="2012.11.23   13:40"/>
	virtual Panic WaitResponse(TaskNumber num);
};