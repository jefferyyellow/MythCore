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

public:
	void init()
	{
        memset(mTaskConfig, NULL, sizeof(mTaskConfig));
	}

	void clear();

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
	virtual void onCreatePlayer(CEntityPlayer& rPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer& rPlayer);
	/// ʱ�亯��
	virtual void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	// �������Ե���������
	int		loadAllTaskConfig(const char* pPath);
	// ���ص�����������
	int		loadTaskConfig(const char* pFilePath);
	// ��������
	void	loadTaskCond(XMLElement* pXmlElem, CTaskCondition& rCondition);

	CTaskConfig* getTaskConfig(unsigned int nTaskID)
	{
		if (nTaskID >= MAX_TASK_ID)
		{
			return NULL;
		}
		return mTaskConfig[nTaskID];
	}

	void		setTaskConfig(unsigned int nTaskID, CTaskConfig* pTaskConfig)
	{
		if (nTaskID >= MAX_TASK_ID)
		{
			return ;
		}

		mTaskConfig[nTaskID] = pTaskConfig;
	}

public:
	void	onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

private:
	CTaskConfig*			mTaskConfig[MAX_TASK_ID];	// ��������
};
#endif