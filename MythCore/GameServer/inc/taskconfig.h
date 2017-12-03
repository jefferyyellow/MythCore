#ifndef __TASKCONFIG_H__
#define __TASKCONFIG_H__
#include "singleton.h"
#include <vector>
using namespace std;
using namespace Myth;
#define MAX_ACCEPT_CONDITION_PARAM		2			// ���Ľ�����������
#define MAX_COMPLETE_CONDITION_PARAM	2			// ���������������
#define MAX_TASK_REWARD_PARAM			2			// ��������������
#define MAX_TASK_ID						4096		// �����������ID
enum EmAcceptCondition
{
	emAccept_None			= 0,		// ��
	emAccept_PreTask		= 1,		// ǰ������, ��0��������ǰ������ID
	emAccept_Level			= 2,		// �ȼ�,��0����������С�ȼ�����1�����������ȼ�
};

enum EmCompleteCondition
{
	emComplete_None				= 0,		// ��
	emComplete_KillOgre			= 1,		// ɱ�ּ��� ����0������ID������1��������Ŀ
	emComplete_HoldItem			= 2,		// ӵ�е��� ����0������ID������1��������Ŀ
};


// ��������
class CAcceptCondition
{
public:
	short			mType;									// ��������
	int				mParam[MAX_ACCEPT_CONDITION_PARAM];		// ����
};

// �������
class CCompleteCondition
{
public:
	short			mType;									// ��������
	int				mParam[MAX_COMPLETE_CONDITION_PARAM];	// ����
};

// ����
class CTaskReward
{
public:
	int				mItemID;								// ����ID(��Ǯ����ʯ������ɵ���)
	int				mItemNum;								// ��������
	int				mParam[MAX_TASK_REWARD_PARAM];
};

// ��������
class CTaskConfig
{
public:
	short					mID;					// ����ID
	short					mType;					// ��������
	short					mRepeated;				// �����ظ����
	int						mAcceptNPCID;			// ��������NPC ID
	int						mSubmitNPCID;			// �������NPC ID
	short					mAcceptMapID;			// ���������ͼID
	short					mSubmitMapID;			// ��������ͼID
	vector<CAcceptCondition> mAcceptCondition;		// ������������
	vector<CCompleteCondition> mCompleteCondition;	// �������
	vector<CTaskReward>		mTaskReward;			// ������
};

// �������ù�����
class CTaskConfigManager : public CSingleton<CTaskConfigManager>
{
	friend class CSingleton <CTaskConfigManager>;
private:
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
		if (nTaskID >= MAX_TASK_ID)
		{
			return NULL;
		}
		return mTaskConfig[nTaskID];
	}

private:
	CTaskConfig*			mTaskConfig[MAX_TASK_ID];	// ��������
};
#endif