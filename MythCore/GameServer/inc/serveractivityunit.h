#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
class CPhaseActivityData
{
public:
	CPhaseActivityData()
	{
		mUniqueID = 0;
		mData = 0;
	}
	~CPhaseActivityData(){}

public:
	int		mUniqueID;			// Î¨Ò»ID
	int		mData;				// Êý¾Ý
};
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
        mPhaseActivityNum = 0;
	}

public:
	CPhaseActivityData*		getPhaseDataByID(int nUniqueID);
	CPhaseActivityData*		getCompleteDataByID(int nUniqueID);
	void					addPhaseActivityData(CPhaseActivityData& rData, CPhaseActivityData& rCompleteData);
	void					setPhaseData(int nUniqueID, int nData);
	void					setPhaseCompleteData(int nUniqueID, int nData);

private:
	CPhaseActivityData	mPhaseData[PHASE_ACTIVITY_NUM];
	CPhaseActivityData	mCompleteData[PHASE_ACTIVITY_NUM];
	int					mPhaseActivityNum;
};
#endif