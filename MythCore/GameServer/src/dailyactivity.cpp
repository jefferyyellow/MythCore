#include "dailyactivity.h"
#include "errcode.h"
void CDailyActivity::init()
{
    mType = 0;
    mID = 0;
    mMinLevel = 0;
    mMaxLevel = 0;
    memset(mStartTime, 0, sizeof(mStartTime));
    memset(mEndTime, 0, sizeof(mEndTime));
    memset(mNoticeTime, 0, sizeof(mNoticeTime));
    mStatus = emDailyActStatus_None;
}

/// ���������ļ�
int CDailyActivity::loadActivity(XMLElement* pActivityElem)
{
	return SUCCESS;
}

/// �õ������ļ�������
const char* CDailyActivity::getConfigFileName()
{
	return NULL;
}

/// �����
void CDailyActivity::start()
{

}

/// �����������
void CDailyActivity::end()
{

}

/// ֪ͨ
void CDailyActivity::notice()
{

}