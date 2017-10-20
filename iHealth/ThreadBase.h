#pragma once

#define WM_COMMUNICATE				WM_USER+1024
#define WM_THREAD_WANT_QUIT			WM_USER+1025	// ����ֱ��ʹ�ò���ϵͳ�ṩ�� WM_QUIT ��Ϣ

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

	bool m_bRefThread;		// ��ǰ�߳��Ƿ�Ϊ�����̣߳���Ϊ�����̣߳��˳�ʱ��Դ������������UI�̣߳�
	ThreadInfo *pInfo;

	List<CTask*>	 m_ResponseTask;	// �Ѿ����յ��ľ��з�����Ϣ������

protected:
	static void ThreadFunc(LPVOID lpParam);

	Panic RunMessageLoop(bool answerTask, TaskNumber num);
	void InitialThread();
	void OnDispose();
	virtual void OnFinishOneTask();

public:
	CThreadBase();
	virtual ~CThreadBase();

	// �ڲ�ʹ��
	void ExecuteTask(CTask *pTask);

	CString GetName();
	TaskNumber GenerateTaskNumber();

	// �ⲿ����
	void Suspend();
	void Resume();

	// �ⲿ�߳���ǰ�̷߳���һ������
	virtual TaskNumber AssignTask(CTask *pTask);
	virtual Panic WaitResponse(TaskNumber num);
};