#ifndef __TASKCONFIG_H__
#define __TASKCONFIG_H__
#include "singleton.h"
#include <vector>
#include "tasktype.h"
using namespace std;
using namespace Myth;
// ��������
class CTaskCondition
{
public:
	CTaskCondition()
	{
		mType = 0;
		memset(mParam, 0, sizeof(mParam));
	}
	~CTaskCondition()
	{
	}

public:
	short			mType;									// ��������
	int				mParam[MAX_ACCEPT_CONDITION_PARAM];		// ����
};

// �����ĵ���
class CTaskRewardItem
{
public:
	CTaskRewardItem()
	{
		mItemID = 0;
		mItemNum = 0;
	}

public:
	int				mItemID;								// ����ID
	int				mItemNum;								// ��������
};

// ������
class CTaskReward
{
public:
	CTaskReward()
	{
		mType = 0;
		memset(mParam, 0, sizeof(mParam));
	}
	~CTaskReward()
	{
	}

public:
	short			mType;									// ��������
	int				mParam[MAX_TASK_REWARD_PARAM];			// ����
};


// ��������
class CTaskConfig
{
public:
	typedef	vector<CTaskCondition>		ACCEPT_CONDITION_LIST;
	typedef vector<CTaskCondition>		COMPLETE_CONDITION_LIST;
	typedef vector<CTaskCondition>			CLOSE_CONDITION_LIST;
	typedef vector<CTaskReward>				TASK_PRIZE_LIST;

public:
	CTaskConfig()
	{
		mID = 0;
	}
	~CTaskConfig()
	{
	}

	short					mID;					// ����ID
	ACCEPT_CONDITION_LIST	mAcceptCondition;		// ������������
	COMPLETE_CONDITION_LIST mCompleteCondition;		// �������
	TASK_PRIZE_LIST			mPrizeList;				// �����б�
	TASK_PRIZE_LIST			mAcceptPrizeList;		// ����������
	CLOSE_CONDITION_LIST	mCloseCondition;		// �ر�����
};

#endif