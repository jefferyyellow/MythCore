#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
#include "serveractivitytype.h"

class CEntityPlayer;


class CServerActPrize
{
public:
	CServerActPrize()
	{
		mItemID = 0;
		mItemNum = 0;
	}

	~CServerActPrize()
	{
	}

	int		mItemID;
	int		mItemNum;
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
        mPrizeTime = 0;
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

    time_t getStartTime(){ return mStartTime;}
    void setStartTime(time_t value){ mStartTime = value;}

    time_t getEndTime(){ return mEndTime;}
    void setEndTime(time_t value){ mEndTime = value;}

    time_t getPrizeTime(){ return mPrizeTime;}
    void setPrizeTime(time_t value){ mPrizeTime = value;}

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}
	// end autocode
public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem)		= 0;
	/// 得到配置文件的名字
	virtual const char* getConfigFileName()					= 0;
	/// 活动开启
	virtual void start()									= 0;
	/// 活动结束
	virtual void end()										= 0;
	/// 刷新玩家数据
	virtual void refreshPlayerData(CEntityPlayer* pPlayer, int nParam) = 0;
	/// 清空玩家数据
	virtual void clearPlayerData(CEntityPlayer* pPlayer)	= 0;
	virtual	void getActivityPrize(CEntityPlayer* pPlayer, int nParam)	= 0;

public:
	/// 类型
	byte				mType;
	/// 子类类型
	byte				mSubType;
	/// ID
	short				mID;
	/// 开始时间
	time_t				mStartTime;
	/// 结束时间 
	time_t				mEndTime;
	/// 领奖时间
	time_t				mPrizeTime;
	/// 开服活动状态 default:emServerActState_End
	EmServerActState	mState;
};

class CCondPrizeData
{
public:
	CCondPrizeData()
	{
		mCondNum = 0;
	}
	~CCondPrizeData()
	{}

	int					mCondNum;
	CServerActPrize		mPrize[ACTIVITY_PRIZE_NUM];
};

/// 条件阶段活动
class CPhaseActivity : public CServerActivity
{
public:
	typedef vector<CCondPrizeData>	VEC_COND_PRIZE_LIST;

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
	virtual void refreshPlayerData(CEntityPlayer* pPlayer, int nParam);
	/// 清空玩家数据
	virtual  void clearPlayerData(CEntityPlayer* pPlayer);
	/// 得到活动奖励
	virtual	void getActivityPrize(CEntityPlayer* pPlayer, int nParam);
public:
	/// 条件奖励列表
	VEC_COND_PRIZE_LIST		mCondPrizeList;
};

#endif
