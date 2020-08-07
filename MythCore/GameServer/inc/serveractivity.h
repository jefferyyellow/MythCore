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
typedef vector<CServerActPrize>	ACT_PRIZE_LIST;

class CServerActivity
{
public:
	CServerActivity()
	{
		
	}
	virtual ~CServerActivity()
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
        mState = emSvrActState_End;
		mAvail = true;
	}

public:
	/// autocode don't edit
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

    EmSvrActState getState(){ return mState;}
    void setState(EmSvrActState value){ mState = value;}

	bool getAvail() const { return mAvail; }
	void setAvail(bool nValue) { mAvail = nValue; }
	// end autocode
public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem)		= 0;
	/// 得到配置文件的名字
	virtual const char* getConfigFileName()					= 0;
	/// 活动开启
	virtual void start()									= 0;
	/// 活动结束的清理
	virtual void end()										= 0;
	/// 清理玩家数据
	virtual	void clearPlayerData(CEntityPlayer& rPlayer)	= 0;
	/// 刷新活动进度
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2) = 0;

protected:
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
	EmSvrActState	mState;
	/// 是否是有效活动
	bool				mAvail;
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

	/// 条件
	int					mCondNum;
	/// 奖励列表
	ACT_PRIZE_LIST		mPrizeList;
};

class CPhasePrizeActivity : public CServerActivity
{
public:
	typedef vector<CCondPrizeData>	COND_PRIZE_DATA_LIST;

public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName() = 0;
	/// 活动开启
	virtual void start() = 0;
	/// 活动结束的清理
	virtual void end() = 0;
	/// 清理玩家数据
	virtual	void clearPlayerData(CEntityPlayer& rPlayer) = 0;
	/// 刷新活动进度
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2) = 0;
	
	/// 新加的虚函数
	/// 得到活动奖励
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex) = 0;
public:
	CCondPrizeData*	getCondPrizeData(uint nIndex)
	{
		if (nIndex >= mCondPrizeList.size())
		{
			return NULL;
		}
		return &mCondPrizeList[nIndex];
	}

protected:
	COND_PRIZE_DATA_LIST			mCondPrizeList;
};

// 累计充值
class CCumulativeRecharge : public CPhasePrizeActivity
{
public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName();
	/// 活动开启
	virtual void start(){};
	/// 活动结束的清理
	virtual void end(){};
	/// 清理玩家数据
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// 刷新活动进度
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2);
	/// 得到活动奖励
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex);
};

/// 累计消费
class CCumulativeConsume : public CPhasePrizeActivity
{
public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName();
	/// 活动开启
	virtual void start(){};
	/// 活动结束的清理
	virtual void end(){};
	/// 清理玩家数据
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// 刷新活动进度
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2);
	/// 得到活动奖励
	virtual	void getActivityPrize(CEntityPlayer& rPlayer, int nIndex);
};


class CRankPrizeData
{
public:
	CRankPrizeData()
	{
		mMinRank = -1;
		mMaxRank = -1;
	}
	~CRankPrizeData()
	{}

	/// 最小排名
	int					mMinRank;
	/// 最大排名
	int					mMaxRank;
	/// 奖励列表
	ACT_PRIZE_LIST		mPrizeList;
};
class CRankActivity : public CServerActivity
{
public:
	typedef vector<CRankPrizeData>	RANK_PRIZE_DATA_LIST;

public:
	/// 加载配置文件
	virtual int loadActivity(XMLElement* pActivityElem);
	/// 得到配置文件的名字
	virtual const char* getConfigFileName();
	/// 活动开启
	virtual void start(){};
	/// 活动结束的清理
	virtual void end(){};
	/// 清理玩家数据
	virtual	void clearPlayerData(CEntityPlayer& rPlayer);
	/// 刷新活动进度
	virtual void refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2){}
	/// 得到活动奖励
	void getActivityPrize(CEntityPlayer& rPlayer);

public:
	RANK_PRIZE_DATA_LIST			mRankPrizeList;
};

#endif
