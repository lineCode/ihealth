/********************************************************************
	Created:	2012.11.12  14:27
	Author:		hps
	
	Purpose:	��ʾһ��������ʹ��ʱ����ʹ�� new �ؼ��ִ��� Task ��ʵ����Task ʵ���ڵ��� Assign() ����ִ�����Ժ󣬻Ḻ������ Task ����Դ
	Remark:		
*********************************************************************/

#pragma once
#include "..\Threads\ThreadBase.h"

class CThreadBase;

class CTask
{
public:
	enum WaitEnums
	{
		NotWait	= 0,
		Wait    = 1
	};

protected:

	// �����ڵ�ǰ�̵߳�������
	TaskNumber m_Number;

	CThreadBase  *m_pProducer;	// ���õ�����Ĵ����ߣ�������ʱ�����ͷ�����Դ
	CThreadBase  *m_pCustomer;	// ��������������ߣ��������ִ���ߣ�������ʱ�����ͷ�����Դ
	void		 *m_pContext;	// ���õ������Ļ�����������ʱ������������Դ

	bool m_bNeedResponse;
	bool m_bUseForResponse;		// ָʾ��ǰ����ֻ����Ϊ֮ǰͬһ�������ŵ�ȡ������
	

public:
	Ext::CPP::Event Command;	// �����ִ�к���
	Panic State;				// ��¼�����״̬��ֻ����Ӧ������Ч��������Ӧ����ʱ����ǰ����� m_Panic �ᴫ�ݵ���Ӧ����Ӧ����� m_Panic

private:
	CTask();
	~CTask();

public:
	static CTask* CreateTask(CThreadBase *pProducer, CThreadBase *pCustomer);
	static void ReleaseTask(CTask *&pTask);
	CTask* CreateResponseTask();

	// �����ڲ�����
	virtual void Do();
	void SetTaskNumber(unsigned long long num);

	CThreadBase* GetProducer();
	CThreadBase* GetCustomer();

	bool IsUseForResponse();
	void SetUseForResponse(bool response);
	bool IsNeedResponse();
	TaskNumber GetTaskNumber();
	CString ToString();

	// ����ǰ������䵽 Owner ���߳��У�����ִ�����Ժ��Զ�����ǰ Task ʵ������Դ
	TaskNumber Assign(void *pContext, bool needResponse);
	static Panic Assign(WaitEnums wait, Panic state, void *pContext, EventHandle handle, CThreadBase *pFrom, CThreadBase *pTo);

	// ָ������ת�������ã�CAdoRow *pRow = pTask->GetAttach<CAdoRow*>();	// pTask ��CTask���͵�ָ�룬Attach ָ��ָ��
	template<typename T>
	T GetContext()
	{
		T pointer = static_cast<T>(m_pContext);
		return pointer;
	}
};