#ifndef __TIMEUNIT_H__
#define __TIMEUNIT_H__
#include "playersubunit.h"
#include "servercommon.h"
#include "entitytimer.h"
#include "messagefactory.h"

class CTimeUnit : public CPlayerSubUnit
{
public:
	CTimeUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CTimeUnit(){}

	void init()
	{
		mLastOffTime = 0;
		mOnTime = 0;
		mLastSaveTime = 0;
		mNewDayTime = 0;
	}

public:
	/// ��������
	void	onHeartBeatRequest(Message* pMessage);

public:
	/// autocode don't edit!!!
	time_t getLastOffTime(){ return mLastOffTime; }
	void setLastOffTime(time_t value){ mLastOffTime = value; }

	time_t getOnTime(){ return mOnTime; }
	void setOnTime(time_t value){ mOnTime = value; }

	time_t getLastSaveTime(){ return mLastSaveTime; }
	void setLastSaveTime(time_t value){ mLastSaveTime = value; }

	CTimerList& getTimerList(){ return mTimerList; }

	time_t getNewDayTime(){ return mNewDayTime; }
	void setNewDayTime(time_t tNewDayTime){ mNewDayTime = tNewDayTime; }
	/// end autocode
private:
	/// �ϴ�����ʱ��
	time_t			mLastOffTime;
	/// �������ʱ��
	time_t			mOnTime;
	/// �ϴδ��̵�ʱ��
	time_t			mLastSaveTime;
	///	��ʱ���б�
	CTimerList		mTimerList;
	/// �µ�һ�����ʱ��
	time_t			mNewDayTime;
	///  ����ʱ��
	time_t			mHeartBeatTime;
	///  ��������ʱ��
	time_t			mHeartBeatCheckTime;
	///	 �������ٴ���
	time_t			mHeartBeatErrCount;
};
#endif