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
/// �����
void CDailyActivity::start()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityStart", mType, mID);
}

/// �����������
void CDailyActivity::end()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityEnd", mType, mID);
}

/// ֪ͨ
void CDailyActivity::notice()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "DailyActivity_ActivityNotice", mType, mID);
}