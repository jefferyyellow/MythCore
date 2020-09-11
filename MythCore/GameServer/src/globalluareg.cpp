#include "globalluareg.h"
#include "scenejob.h"
#include "fileutility.h"
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


	lua_tinker::class_add<CDir>(L, "CDir");
	lua_tinker::class_con<CDir>(L, lua_tinker::constructor<CDir>);
	lua_tinker::class_def<CDir>(L, "findFirstFile", &CDir::findFirstFileLua);
	lua_tinker::class_def<CDir>(L, "nextFile", &CDir::nextFileLua);


}