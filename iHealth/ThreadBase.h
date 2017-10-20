#pragma once

#define WM_COMMUNICATE				WM_USER+1024
#define WM_THREAD_WANT_QUIT			WM_USER+1025	// 不能直接使用操作系统提供的 WM_QUIT 消息

class CTask;
struct ThreadInfo;

class CThreadBase
{
protected:
	CString m_Name;
	HANDLE				m_hMutex;
	TaskNumber			m_TopTaskNumber;

	HANDLE m_HThread;
	DWORD  m_ThreadID;
	bool   m_bIsSuspend;
	bool   m_bReceiveQuitMessage;

	bool m_bRefThread;		// 当前线程是否为引用线程，若为引用线程，退出时资源不能清理（比如UI线程）
	ThreadInfo *pInfo;

	List<CTask*>	 m_ResponseTask;	// 已经接收到的具有反馈信息的任务

protected:
	static void ThreadFunc(LPVOID lpParam);

	Panic RunMessageLoop(bool answerTask, TaskNumber num);
	void InitialThread();
	void OnDispose();
	virtual void OnFinishOneTask();

public:
	CThreadBase();
	virtual ~CThreadBase();

	// 内部使用
	void ExecuteTask(CTask *pTask);

	CString GetName();
	TaskNumber GenerateTaskNumber();

	// 外部调用
	void Suspend();
	void Resume();

	// 外部线程向当前线程分配一个任务
	virtual TaskNumber AssignTask(CTask *pTask);
	virtual Panic WaitResponse(TaskNumber num);
};