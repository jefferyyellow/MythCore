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

/// 加载配置文件
int CDailyActivity::loadActivity(XMLElement* pActivityElem)
{
	return SUCCESS;
}

/// 得到配置文件的名字
const char* CDailyActivity::getConfigFileName()
{
	return NULL;
}

/// 活动开启
void CDailyActivity::start()
{

}

/// 活动结束的清理
void CDailyActivity::end()
{

}

/// 通知
void CDailyActivity::notice()
{

}