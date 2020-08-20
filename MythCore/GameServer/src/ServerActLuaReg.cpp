#include "ServerActLuaReg.h"
#include "scenejob.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
#include "serveractmodule.h"

void CServerActLuaReg::reglua()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();

	// 全局函数得到CServerActModule的实例
	lua_tinker::def(L, "getServerActModule", &CServerActModule::Inst);

	lua_tinker::class_add<CServerActModule>(L, "CServerActModule");
	lua_tinker::class_def<CServerActModule>(L, "checkActOpen", &CServerActModule::checkActOpen);
	lua_tinker::class_def<CServerActModule>(L, "checkActPrizeTime", &CServerActModule::checkActPrizeTime);

}