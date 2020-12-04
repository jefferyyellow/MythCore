#include "dailyactivity.h"
#include "errcode.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
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
/// 活动开启
void CDailyActivity::start()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityStart", mType, mID);
}

/// 活动结束的清理
void CDailyActivity::end()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityEnd", mType, mID);
}

/// 通知
void CDailyActivity::notice()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityNotice", mType, mID);
}