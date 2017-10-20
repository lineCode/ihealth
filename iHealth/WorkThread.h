#pragma once
#include "ThreadBase.h"

class CWorkThread : public CThreadBase
{
private:
	static HANDLE sm_hMutex;	// 同步多个Work线程同时访问 m_WorkerThreadPool 的互斥锁
	static int sm_WorkerCount;
	static List<CWorkThread*> sm_AvailableWorker;
	static List<CWorkThread*> sm_BusyWorker;

	bool m_bAutoReuse;	// 自动重用，即在完成一个任务后，自动地将当前线程回收到线程池中。默认为 true

public:
	static CWorkThread* GetWorker();
	static void Dispose();
	static void Destroy();

	bool IsAutoReuse();
	void SetAutoReuse(bool isAuto);

	// 手动地将当前线程回收到线程池中
	void Reuse();

	// 重写，在执行完一个任务后，将当前线程回收到线程池中
	virtual void OnFinishOneTask();

private:
	CWorkThread();
	~CWorkThread();

	static void AddWorker(std::List<CWorkThread*> &list, CWorkThread *pWork);
	static void RemoveWorker(std::List<CWorkThread*> &list, CWorkThread *pWork);
};