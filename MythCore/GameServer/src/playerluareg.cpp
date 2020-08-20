#include "playerluareg.h"
#include "entityplayer.h"
#include "scenejob.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
void CPlayerLuaReg::reglua()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	// CEntityPlayer本身接口注册
	lua_tinker::class_add<CEntityPlayer>(L, "CEntityPlayer");
	lua_tinker::class_def<CEntityPlayer>(L, "getRoleID", &CEntityPlayer::getRoleID);
	lua_tinker::class_def<CEntityPlayer>(L, "getLevel", &CEntityPlayer::getLevel);
	lua_tinker::class_def<CEntityPlayer>(L, "getPropertyUnit", &CEntityPlayer::getPropertyUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getItemUnit", &CEntityPlayer::getItemUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getTaskUnit", &CEntityPlayer::getTaskUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getSkillUnit", &CEntityPlayer::getSkillUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getServerActUnit", &CEntityPlayer::getServerActUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getVIPUnit", &CEntityPlayer::getVIPUnit);
	lua_tinker::class_def<CEntityPlayer>(L, "getTimeUnit", &CEntityPlayer::getTimeUnit);

	// 玩家单元基类
	lua_tinker::class_add<CPlayerSubUnit>(L, "CPlayerSubUnit");
	lua_tinker::class_def<CPlayerSubUnit>(L, "getPlayer", &CPlayerSubUnit::getPlayer);

	// 道具的查看itemluareg.cpp


	// 开服活动单元
	lua_tinker::class_add<CServerActUnit>(L, "CServerActUnit");
	lua_tinker::class_inh<CServerActUnit, CPlayerSubUnit>(L);
	lua_tinker::class_def<CServerActUnit>(L, "getActData", &CServerActUnit::getActData);
	lua_tinker::class_def<CServerActUnit>(L, "setActData", &CServerActUnit::setActData);
	lua_tinker::class_def<CServerActUnit>(L, "getActDataBit", &CServerActUnit::getActDataBit);
	lua_tinker::class_def<CServerActUnit>(L, "setActDataBit", &CServerActUnit::setActDataBit);
	lua_tinker::class_def<CServerActUnit>(L, "clearActDataBit", &CServerActUnit::clearActDataBit);
	lua_tinker::class_def<CServerActUnit>(L, "getActBit", &CServerActUnit::getActBit);
	lua_tinker::class_def<CServerActUnit>(L, "setActBit", &CServerActUnit::setActBit);
	lua_tinker::class_def<CServerActUnit>(L, "clearBit", &CServerActUnit::clearBit);


}