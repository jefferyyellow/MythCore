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

	/// autocode don't edit
	/// end autocode

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