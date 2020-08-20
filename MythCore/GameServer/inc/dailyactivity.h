#ifndef __DAILYACTIVITY_H__
#define __DAILYACTIVITY_H__
#define MAX_ACTIVITY_TIMES		4
#include "servercommon.h"
#include "dailyactivitytype.h"
class CDailyActTime
{
public:
	int					mID;			// �ID
	int					mTime;			// ʱ��
	int					mTimeIndex;		// ʱ������
	EmDailyActStatus	mStatus;		// �״̬
};

class CDailyActivity
{
public:
	CDailyActivity()
	{
	}
	~CDailyActivity()
	{
	}
	
	void init();

public:
	/// ���ò���
	/// �����
	short				mType;
	/// ID
	short				mID;
	/// ��С�ȼ�
	byte				mMinLevel;
	/// ���ȼ�
	byte				mMaxLevel;
	/// ��ʼʱ��
	int					mStartTime[MAX_ACTIVITY_TIMES];
	/// ����ʱ�� 
	int					mEndTime[MAX_ACTIVITY_TIMES];
	/// ֪ͨʱ��
	int					mNoticeTime[MAX_ACTIVITY_TIMES];

public:
	EmDailyActStatus GetStatus() const { return mStatus; }
	void SetStatus(EmDailyActStatus nValue) { mStatus = nValue; }

public:
	/// �����
	void start();
	/// �����������
	void end();
	/// ֪ͨ
	void notice();

public:
	/// autocode, don't edit!!!
    EmDailyActStatus getStatus(){ return mStatus;}
    void setStatus(EmDailyActStatus value){ mStatus = value;}
	/// end autocode
private:
	/// ���ݲ��� default:emDailyActStatus_None
	EmDailyActStatus	mStatus;
};


#endif