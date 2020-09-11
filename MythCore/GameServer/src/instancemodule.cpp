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
/// ����������
void CInstanceModule::onLaunchServer()
{

}

/// ������ɼ��
bool CInstanceModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CInstanceModule::onLaunchComplete()
{

}

/// �˳�������
void CInstanceModule::onExitServer()
{

}

/// ��һ�쵽��
void CInstanceModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CInstanceModule::onNewWeekCome()
{

}

/// ����ʵ��
void CInstanceModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CInstanceModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CInstanceModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
void CInstanceModule::onLoadConfig()
{
	lua_State* L = CSceneJob::Inst()->getLuaState();
	lua_tinker::dofile(L, "gameserverconfig/script/instance/instance.lua");

	loadAllInstConfig("gameserverconfig/instance/*.lua", "gameserverconfig/instance/");
}

// �������еĸ�������
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

// �������Ƿ��ܽ�����
int CInstanceModule::checkPlayerEnterInstance(CEntityPlayer& rPlayer, int nInstanceType, int nInstanceID)
{
	// ͨ�ü��
	int nResult = SUCCESS;

	lua_State* L = CSceneJob::Inst()->getLuaState();
	nResult = lua_tinker::call<int>(L, "Instance_CheckPlayerEnterFunc", nInstanceType, nInstanceID, &rPlayer);
	return nResult;
}

// ��ҽ��븱��������
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

// ��ҽ��븱���Ļ�Ӧ
void CInstanceModule::sendEnterInstanceResponse(CEntityPlayer& rPlayer, int nResult, int nInstanceID)
{
	CEnterInstanceResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_instanceid(nInstanceID);

	CSceneJob::Inst()->send2Player(rPlayer, ID_S2C_RESPONSE_ENTER_INSTANCE, &tResponse);
}

// ����뿪����������
void CInstanceModule::onLeaveInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	
}

// ����뿪�����Ļ�Ӧ
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