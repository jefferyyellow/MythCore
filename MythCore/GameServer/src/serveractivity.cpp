#include "serveractivity.h"
#include "errcode.h"
#include "serveractivityunit.h"
#include "entityplayer.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"

void CServerActivity::end()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_EndFunc", getType(), getID());
}

void CServerActivity::refreshProcess(CEntityPlayer& rPlayer, int nParam1, int nParam2)
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_RefreshFunc", getType(), getID(), &rPlayer, nParam1, nParam2);
}

void CServerActivity::clearPlayerData(CEntityPlayer& rPlayer)
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_ClearPlayerData", getType(), getID(), &rPlayer);
}

const char* CServerActivity::getConfigName()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//return lua_tinker::call<const char*>(L, "ServerActivity_GetConfig", getType());
	return NULL;
}