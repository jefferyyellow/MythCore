#include "globalluareg.h"
#include "scenejob.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"

void CGlobalLuaReg::regLua()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::def(L, "getSceneJob", &CSceneJob::Inst);
	lua_tinker::class_add<CSceneJob>(L, "CSceneJob");
	lua_tinker::class_def<CSceneJob>(L, "send2Player", &CSceneJob::send2PlayerLua);
}