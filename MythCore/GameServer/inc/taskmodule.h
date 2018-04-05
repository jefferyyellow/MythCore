#ifndef __TASKMODULE_H__
#define __TASKMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
#include "taskunit.h"
using namespace Myth;
class CEntityPlayer;
class CTaskConfig;
class CTaskModule : public CLogicModule, public CSingleton < CTaskModule >
{
	friend class CSingleton <CTaskModule> ;
private:
	CTaskModule()
	{
		init();
	}
	~CTaskModule()
	{
	}

	int init()
	{
        memset(mTaskConfig, NULL, sizeof(mTaskConfig));
	}

	/// ����������
	virtual void onLaunchServer();
	/// ������ɼ��
	virtual bool onCheckLaunch();
	/// �����������ɹ�
	virtual void onLaunchComplete();
	/// �˳�������
	virtual void onExitServer();
	/// ��һ�쵽��
	virtual void onNewDayCome();
	/// ��һ�ܵ���
	virtual void onNewWeekCome();
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	virtual void onTimer(unsigned int nTickOffset);

public:
	// �������Ե���������
	void	loadAllTaskConfig(const char* pPath);
	// ���ص�����������
	void	loadTaskConfig(const char* pFileName);

	CTaskConfig*			getTaskConfig(unsigned int nTaskID)
	{
		if (nTaskID >= MAX_TASK_ID)
		{
			return NULL;
		}
		return mTaskConfig[nTaskID];
	}

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

private:
	CTaskConfig*			mTaskConfig[MAX_TASK_ID];	// ��������
};
#endif