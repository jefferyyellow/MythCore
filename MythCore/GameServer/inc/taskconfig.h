#ifndef __TASKCONFIG_H__
#define __TASKCONFIG_H__
#include <vector>
using namespace std;
#define MAX_ACCEPT_CONDITION_PARAM		2			// ���Ľ�����������
#define MAX_COMPLETE_CONDITION_PARAM	2			// ���������������
#define MAX_TASK_REWARD_PARAM			2			// ��������������
#define MAX_ALL_TASK_ID					3000		// �����������ID
// ��������
class CAcceptCondition
{
	short			mType;									// ��������
	int				mParam[MAX_ACCEPT_CONDITION_PARAM];		// ����
};

// �������
class CCompleteCondition
{
	short			mType;									// ��������
	int				mParam[MAX_COMPLETE_CONDITION_PARAM];	// ����
};

// ����
class CTaskReward
{
	short			mType;									// ��������
	int				mParam[MAX_TASK_REWARD_PARAM];			// ����
};

// ��������
class CTaskConfig
{
public:
	short					mID;					// ����ID
	short					mType;					// ��������
	int						mAcceptNPCID;			// ��������NPC ID
	int						mSubmitNPCID;			// �������NPC ID
	short					mAcceptMapID;			// ���������ͼID
	short					mSubmitMapID;			// ��������ͼID
	vector<CAcceptCondition> mAcceptCondition;		// ������������
	vector<CCompleteCondition> mCompleteCondition;	// �������
	vector<CTaskReward>		mTaskReward;			// ������
};

// �������ù�����
class CTaskConfigManager
{
public:
	CTaskConfigManager()
	{
	}
	~CTaskConfigManager()
	{
	}

public:
	void					loadTaskConfig(const char* pPath);
	CTaskConfig*			getTaskConfig(unsigned int nTaskID)
	{
		if (nTaskID >= MAX_ALL_TASK_ID)
		{
			return NULL;
		}
		return mTaskConfig[nTaskID];
	}

private:
	CTaskConfig*			mTaskConfig[MAX_ALL_TASK_ID];	// ��������
};
#endif