#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
class CEntityPlayer;
enum EmServerActState
{
	emServerActState_End		= 0,		// 结束状态
	emServerActState_Start		= 1,		// 开始状态
	emServerActState_Prize		= 2,		// 奖励状态
};

class CServerActPrize
{
public:
	int		mItemID[ACTIVITY_PRIZE_NUM];
	int		mItemNum[ACTIVITY_PRIZE_NUM];
};
class CServerActivity
{
public:
	CServerActivity()
	{
		
	}
	~CServerActivity()
	{
		
	}

	void init()
	{
        mType = 0;
        mSubType = 0;
        mID = 0;
        mStartTime = 0;
        mEndTime = 0;
        mState = emServerActState_End;
	}

public:
	// autocode
    byte getType(){ return mType;}
    void setType(byte value){ mType = value;}

    byte getSubType(){ return mSubType;}
    void setSubType(byte value){ mSubType = value;}

    short getID(){ return mID;}
    void setID(short value){ mID = value;}

    int getStartTime(){ return mStartTime;}
    void setStartTime(int value){ mStartTime = value;}

    int getEndTime(){ return mEndTime;}
    void setEndTime(int value){ mEndTime = value;}

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}
	// end autocode
public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName();
	/// 活动开启
	virtual void start();
	/// 活动结束
	virtual void end();
	/// 刷新玩家数据
	virtual void refreshPlayerData(CEntityPlayer* pPlayer);
	/// 清空玩家数据
	virtual  void clearPlayerData(CEntityPlayer* pPlayer);

public:
	/// 类型
	byte				mType;
	/// 子类类型
	byte				mSubType;
	/// ID
	short				mID;
	/// 开始时间
	int					mStartTime;
	/// 结束时间 
	int					mEndTime;
	/// 开服活动状态 default:emServerActState_End
	EmServerActState	mState;
};

/// 条件阶段活动
class CPhaseActivity : public CServerActivity
{
public:
	typedef vector<int>					VEC_COND_LIST;
	typedef vector<CServerActPrize>		VEC_PRIZE_LIST;
public:
	CPhaseActivity()
	{
		
	}
	~CPhaseActivity()
	{
		
	}

public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName();
	/// 活动开启
	virtual void start();
	/// 活动结束
	virtual void end();
	/// 刷新玩家数据
	virtual void refreshPlayerData(CEntityPlayer* pPlayer);
	/// 清空玩家数据
	virtual  void clearPlayerData(CEntityPlayer* pPlayer);

public:
	/// 条件列表
	VEC_COND_LIST			mCondList;
	/// 奖励列表
	VEC_PRIZE_LIST			mPrizeList;
};

#endif
