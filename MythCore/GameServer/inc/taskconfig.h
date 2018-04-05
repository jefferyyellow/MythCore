#ifndef __TASKCONFIG_H__
#define __TASKCONFIG_H__
#include "singleton.h"
#include "tasktype.h"
#include "servercommon.h"
using namespace std;
using namespace Myth;
// 接受条件
class CTaskCondition
{
public:
	CTaskCondition()
	{
		init();
	}
	~CTaskCondition()
	{

	}

	void init()
	{
        mType = 0;
        memset(mParam, 0, sizeof(mParam));
	}

public:
	short			mType;									// 条件类型
	int				mParam[MAX_ACCEPT_CONDITION_PARAM];		// 参数
};

// 奖励的道具
class CTaskRewardItem
{
public:
	CTaskRewardItem()
	{
		init();
	}

	void init()
	{
        mItemID = 0;
        mItemNum = 0;
	}

public:
	int				mItemID;								// 道具ID
	int				mItemNum;								// 道具数量
};

// 任务奖励
class CTaskReward
{
public:
	CTaskReward()
	{
		init();
	}
	~CTaskReward()
	{
	}
	void init()
	{
        mType = 0;
        memset(mParam, 0, sizeof(mParam));
	}

public:
	short			mType;									// 奖励类型
	int				mParam[MAX_TASK_REWARD_PARAM];			// 参数
};


// 任务配置
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
		init();
	}
	~CTaskConfig()
	{
	}
	void init()
	{
        mID = 0;
	}

	short					mID;					// 任务ID
	ACCEPT_CONDITION_LIST	mAcceptCondition;		// 接受任务条件
	COMPLETE_CONDITION_LIST mCompleteCondition;		// 完成条件
	TASK_PRIZE_LIST			mPrizeList;				// 奖励列表
	TASK_PRIZE_LIST			mAcceptPrizeList;		// 接受任务奖励
	CLOSE_CONDITION_LIST	mCloseCondition;		// 关闭条件
};

#endif