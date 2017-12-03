#ifndef __TASKCONFIG_H__
#define __TASKCONFIG_H__
#include "singleton.h"
#include <vector>
using namespace std;
using namespace Myth;
#define MAX_ACCEPT_CONDITION_PARAM		2			// 最大的接受条件参数
#define MAX_COMPLETE_CONDITION_PARAM	2			// 最大的完成条件参数
#define MAX_TASK_REWARD_PARAM			2			// 最大的任务奖励参数
#define MAX_TASK_ID						4096		// 所有任务最大ID
enum EmAcceptCondition
{
	emAccept_None			= 0,		// 无
	emAccept_PreTask		= 1,		// 前置任务, 第0个参数，前置任务ID
	emAccept_Level			= 2,		// 等级,第0个参数，最小等级，第1个参数，最大等级
};

enum EmCompleteCondition
{
	emComplete_None				= 0,		// 无
	emComplete_KillOgre			= 1,		// 杀怪计数 参数0：怪物ID，参数1：怪物数目
	emComplete_HoldItem			= 2,		// 拥有道具 参数0：道具ID，参数1：道具数目
};


// 接受条件
class CAcceptCondition
{
public:
	short			mType;									// 条件类型
	int				mParam[MAX_ACCEPT_CONDITION_PARAM];		// 参数
};

// 完成条件
class CCompleteCondition
{
public:
	short			mType;									// 条件类型
	int				mParam[MAX_COMPLETE_CONDITION_PARAM];	// 参数
};

// 奖励
class CTaskReward
{
public:
	int				mItemID;								// 道具ID(金钱，钻石都抽象成道具)
	int				mItemNum;								// 道具数量
	int				mParam[MAX_TASK_REWARD_PARAM];
};

// 任务配置
class CTaskConfig
{
public:
	short					mID;					// 任务ID
	short					mType;					// 任务类型
	short					mRepeated;				// 可以重复完成
	int						mAcceptNPCID;			// 接受任务NPC ID
	int						mSubmitNPCID;			// 完成任务NPC ID
	short					mAcceptMapID;			// 接受任务地图ID
	short					mSubmitMapID;			// 完成任务地图ID
	vector<CAcceptCondition> mAcceptCondition;		// 接受任务条件
	vector<CCompleteCondition> mCompleteCondition;	// 完成条件
	vector<CTaskReward>		mTaskReward;			// 任务奖励
};

// 任务配置管理器
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
	CTaskConfig*			mTaskConfig[MAX_TASK_ID];	// 任务配置
};
#endif