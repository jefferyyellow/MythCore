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
		mInvalid = false;
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

    EmServerActState getState(){ return mState;}
    void setState(EmServerActState value){ mState = value;}

	bool getInvalid() const { return mInvalid; }
	void setInvalid(bool nValue) { mInvalid = nValue; }

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
	EmServerActState	mState;
	/// 是否是有效活动
	bool				mInvalid;
};

#endif
