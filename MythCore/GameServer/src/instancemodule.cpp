#include "instancemodule.h"
#include "instance.h"
#include "objpool.h"
#include "instancemodule.hxx.pb.h"
#include "locallogjob.h"
#include "errcode.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
#include "scenejob.h"
#include "fileutility.h"
/// 启动服务器
void CInstanceModule::onLaunchServer()
{

}

/// 启动完成检查
bool CInstanceModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CInstanceModule::onLaunchComplete()
{

}

/// 退出服务器
void CInstanceModule::onExitServer()
{

}

/// 新一天到来
void CInstanceModule::onNewDayCome()
{

}

/// 新一周到来
void CInstanceModule::onNewWeekCome()
{

}

/// 建立实体
void CInstanceModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CInstanceModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CInstanceModule::onTimer(unsigned int nTickOffset)
{

}

/// 加载配置文件
void CInstanceModule::onLoadConfig()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/instance/instance.lua");

	loadAllInstConfig("gameserverconfig/instance/*.lua", "gameserverconfig/instance/");
}

// 加载所有的副本配置
void CInstanceModule::loadAllInstConfig(const char* pFindPath, const char* pPath)
{
	if (NULL == pFindPath || NULL == pPath)
	{
		return;
	}

	lua_State* L = CSceneJob::Inst()->getLuaState();


	CDir tDir;
	char tPathFile[STR_LENGTH_256] = { 0 };
	char tFileName[STR_LENGTH_256] = { 0 };
	tDir.findFirstFile(pFindPath, tFileName, sizeof(tFileName));
	while (tFileName[0] != '\0')
	{
		snprintf(tPathFile, sizeof(tPathFile) - 1, "%s%s", pPath, tFileName);
		lua_tinker::dofile(L, tPathFile);
		CInstanceConfig* pNewInstConfig = new CInstanceConfig;
		if (NULL == pNewInstConfig)
		{
			continue;
		}
		pNewInstConfig->loadConfig();
		if (pNewInstConfig->mID <= 0 || pNewInstConfig->mID >= MAX_INSTANCE_ID)
		{
			delete pNewInstConfig;
			continue;
		}

		lua_tinker::call<int>(L, "initInstLuaConfig", pNewInstConfig->mID);
		mAllInstConfig[pNewInstConfig->mID] = pNewInstConfig;
		tDir.nextFile(tFileName, sizeof(tFileName));
	}

}

void CInstanceModule::onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_ENTER_INSTANCE:
		{
			onEnterInstanceRequest(rPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_LEAVE_INSTANCE:
		{
			onLeaveInstanceRequest(rPlayer, pMessage);
			break;
		}
	}
}

// 检查玩家是否能进副本
int CInstanceModule::checkPlayerEnterInstance(CEntityPlayer& rPlayer, int nInstanceType, int nInstanceID)
{
	// 通用检查
	int nResult = SUCCESS;

	lua_State* L = CSceneJob::Inst()->getLuaState();
	nResult = lua_tinker::call<int>(L, "Instance_CheckPlayerEnterFunc", nInstanceType, nInstanceID, &rPlayer);
	return nResult;
}

// 玩家进入副本的请求
void CInstanceModule::onEnterInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CEnterInstanceRequest* pRequest = (CEnterInstanceRequest*)pMessage;

	int nInstanceID = pRequest->instanceid();
	if (nInstanceID <= 0 || nInstanceID >= MAX_INSTANCE_ID)
	{
		return;
	}

	CInstanceConfig* pInstanceConfig = mAllInstConfig[nInstanceID];
	if (NULL == pInstanceConfig)
	{
		return;
	}

	int nResult = checkPlayerEnterInstance(rPlayer, pInstanceConfig->mType, nInstanceID);
	if (SUCCESS != nResult)
	{
		return;
	}

	CInstance* pInstance = createInstance(nInstanceID);
	if (NULL == pInstance)
	{
		return;
	}
	pInstance->create(pInstanceConfig);

	pInstance->playerEnter(rPlayer);

	sendEnterInstanceResponse(rPlayer, SUCCESS, nInstanceID);
}

// 玩家进入副本的回应
void CInstanceModule::sendEnterInstanceResponse(CEntityPlayer& rPlayer, int nResult, int nInstanceID)
{
	CEnterInstanceResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_instanceid(nInstanceID);

	CSceneJob::Inst()->send2Player(rPlayer, ID_S2C_RESPONSE_ENTER_INSTANCE, &tResponse);
}

// 玩家离开副本的请求
void CInstanceModule::onLeaveInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	
}

// 玩家离开副本的回应
void CInstanceModule::sendLeaveInstanceResponse(CEntityPlayer& rPlayer, int nResult)
{
	CLeaveInstanceResponse tResponse;
	tResponse.set_result(nResult);

	CSceneJob::Inst()->send2Player(rPlayer, ID_S2C_RESPONSE_LEAVE_INSTANCE, &tResponse);
}

CInstance* CInstanceModule::createInstance(int nInstanceID)
{
	CInstance* pInstance = (CInstance*)CObjPool::Inst()->allocObj(emObjType_Instance);
	if (NULL == pInstance)
	{
		return NULL;
	}

	pInstance->setInstanceID(nInstanceID);
	return pInstance;
}