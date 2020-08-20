#include "itemluareg.h"
#include "scenejob.h"
#include "itemunit.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"

void CItemLuaReg::regLua()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();

	// 道具整理类
	lua_tinker::class_add<CItemArrange>(L, "CItemArrange");
	lua_tinker::class_con<CItemArrange>(L, lua_tinker::constructor<CItemArrange>);
	lua_tinker::class_def<CItemArrange>(L, "getItemID", &CItemArrange::getItemID);
	lua_tinker::class_def<CItemArrange>(L, "setItemID", &CItemArrange::setItemID);
	lua_tinker::class_def<CItemArrange>(L, "getIDPtr", &CItemArrange::getIDPtr);

	lua_tinker::class_def<CItemArrange>(L, "getItemNum", &CItemArrange::getItemNum);
	lua_tinker::class_def<CItemArrange>(L, "setItemNum", &CItemArrange::setItemNum);
	lua_tinker::class_def<CItemArrange>(L, "getNumPtr", &CItemArrange::getNumPtr);

	lua_tinker::class_def<CItemArrange>(L, "getLevel", &CItemArrange::getLevel);
	lua_tinker::class_def<CItemArrange>(L, "setLevel", &CItemArrange::setLevel);

	lua_tinker::class_def<CItemArrange>(L, "getQuality", &CItemArrange::getQuality);
	lua_tinker::class_def<CItemArrange>(L, "setQuality", &CItemArrange::setQuality);
	lua_tinker::class_def<CItemArrange>(L, "GetCount", &CItemArrange::GetCount);
	lua_tinker::class_def<CItemArrange>(L, "addItem", &CItemArrange::addItem);
	lua_tinker::class_def<CItemArrange>(L, "clear", &CItemArrange::clear);


	lua_tinker::class_add<CItemUnit>(L, "CItemUnit");
	lua_tinker::class_def<CItemUnit>(L, "getPlayer", &CItemUnit::getPlayer);
	lua_tinker::class_def<CItemUnit>(L, "checkItemSpace", &CItemUnit::checkItemSpace);
	lua_tinker::class_def<CItemUnit>(L, "insertAllItem", &CItemUnit::insertAllItem);

}