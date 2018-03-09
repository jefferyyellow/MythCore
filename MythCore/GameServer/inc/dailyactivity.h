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
	/// ���������ļ�
	virtual int loadActivity(XMLElement* pActivityElem) = 0;
	/// �õ������ļ�������
	virtual const char* getConfigFileName() = 0;
	/// �����
	virtual void start() = 0;
	/// �����������
	virtual void end() = 0;

private:
	/// ���ݲ���
	EmDailyActStatus	mStatus;
};


#endif