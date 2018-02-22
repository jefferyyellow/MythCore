#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
#include "servercommon.h"
class CServerActData
{
public:
	CServerActData()
	{
		init();
	}
	~CServerActData(){}

	void	init()
	{
        mUniqueID = 0;
        mStartTime = 0;
        mParam1 = 0;
        mParam2 = 0;
	}

	// autocode
    int getUniqueID(){ return mUniqueID;}
    void setUniqueID(int value){ mUniqueID = value;}

    time_t getStartTime(){ return mStartTime;}
    void setStartTime(time_t value){ mStartTime = value;}

    int getParam1(){ return mParam1;}
    void setParam1(int value){ mParam1 = value;}

    int getParam2(){ return mParam2;}
    void setParam2(int value){ mParam2 = value;}
	// end autocode

public:
	/// 唯一ID
	int		mUniqueID;
	/// 开始时间
	time_t	mStartTime;
	/// 参数1
	int		mParam1;
	/// 参数2
	int		mParam2;
};

/// 活动单元
class CServerActivityUnit : public CPlayerSubUnit
{
public:
	CServerActivityUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		
	}
	~CServerActivityUnit()
	{
		init();
	}

	void	init()
	{
        mActivityNum = 0;
	}

public:
	CServerActData*		getServerActData(int nUniqueID);
	void				addServerActData(CServerActData& rData);
	int					removeServerActData(int nPos);
	void				checkAllServerAct();
	// autocode
    CServerActData* getActivityData(int nIndex)
    {
        if(nIndex < 0 || nIndex >= PHASE_ACTIVITY_NUM)
        {
            return NULL;
        }
        return &mActivityData[nIndex];
    }
    void setActivityData(int nIndex, CServerActData& value)
    {
        if(nIndex < 0 || nIndex >= PHASE_ACTIVITY_NUM)
        {
            return;
        }
        mActivityData[nIndex] = value;
    }

    int getActivityNum(){ return mActivityNum;}
    void setActivityNum(int value){ mActivityNum = value;}
	// end autocode
private:
	CServerActData		mActivityData[PHASE_ACTIVITY_NUM];
	int					mActivityNum;
};
#endif