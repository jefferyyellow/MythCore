#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "servercommon.h"
#include "serveractivitytype.h"

class CEntityPlayer;
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

	const char* getEndFunc(){return mEndFunc;}
	void setEndFunc(const char* pEndFunc)
	{
		if (NULL == pEndFunc)
		{
			return;
		}
		strncpy(mEndFunc, pEndFunc, sizeof(mEndFunc) - 1);
	}

	const char* getClearPlayerData(){ return mEndFunc; }
	void setClearPlayerData(const char* pClearPlayerData)
	{
		if (NULL == pClearPlayerData)
		{
			return;
		}
		strncpy(mClearPlayerData, pClearPlayerData, sizeof(mClearPlayerData) - 1);
	}

	const char* getRefreshProcess(){ return mRefreshProcess; }
	void setRefreshProcess(const char* pRefreshProcess)
	{
		if (NULL == pRefreshProcess)
		{
			return;
		}
		strncpy(mRefreshProcess, pRefreshProcess, sizeof(mRefreshProcess) - 1);
	}
	// end autocode

protected:
	/// ID
	short				mID;
	/// 类型
	byte				mType;
	/// 子类类型
	byte				mSubType;
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
	/// 结束函数		
	char				mEndFunc[STR_LENGTH_128];
	/// 清理玩家数据函数
	char				mClearPlayerData[STR_LENGTH_128];
	/// 刷新活动进度
	char				mRefreshProcess[STR_LENGTH_128];
};


#endif
