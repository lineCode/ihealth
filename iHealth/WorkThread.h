#pragma once
#include "ThreadBase.h"

class CWorkThread : public CThreadBase
{
private:
	static HANDLE sm_hMutex;	// ͬ�����Work�߳�ͬʱ���� m_WorkerThreadPool �Ļ�����
	static int sm_WorkerCount;
	static List<CWorkThread*> sm_AvailableWorker;
	static List<CWorkThread*> sm_BusyWorker;

	bool m_bAutoReuse;	// �Զ����ã��������һ��������Զ��ؽ���ǰ�̻߳��յ��̳߳��С�Ĭ��Ϊ true

public:
	static CWorkThread* GetWorker();
	static void Dispose();
	static void Destroy();

	bool IsAutoReuse();
	void SetAutoReuse(bool isAuto);

	// �ֶ��ؽ���ǰ�̻߳��յ��̳߳���
	void Reuse();

	// ��д����ִ����һ������󣬽���ǰ�̻߳��յ��̳߳���
	virtual void OnFinishOneTask();

private:
	CWorkThread();
	~CWorkThread();

	static void AddWorker(std::List<CWorkThread*> &list, CWorkThread *pWork);
	static void RemoveWorker(std::List<CWorkThread*> &list, CWorkThread *pWork);
};