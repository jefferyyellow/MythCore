#include "instance.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"
#include "timemanager.h"
void CInstanceConfig::loadConfig()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::table table(L, "InstanceConfig");

	//mID = table.get<int>("ID");
	//mType = table.get<int>("Type");
	//mTime = table.get<int>("Time");

	//// lua����������1��ʼ
	//lua_tinker::table tMapTable = table.get<lua_tinker::table>("MapID");
	//for (uint i = 1; i <= lua_rawlen(L, -1) && i <= MAX_INSTANCE_MAP_NUM; ++ i)
	//{
	//	mMapId[i - 1] = tMapTable.get<int>(i);
	//}
}

/// ����
void CInstance::create(CInstanceConfig* pInstanceConfig)
{
	//if (NULL == pInstanceConfig)
	//{
	//	return;
	//}

	//if (mStatus == emInstanceStatus_Init)
	//{
	//	return;
	//}

	//mType = (EmInstanceType)pInstanceConfig->mType;
	//mCreateTime = CTimeManager::Inst()->getCurrTime();
	//mExpiredTime = mCreateTime + pInstanceConfig->mTime;

	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "Instance_CreateFunc", mType, mInstanceID);
}

/// ����
void CInstance::end()
{
	//if (mStatus == emInstanceStatus_End)
	//{
	//	return;
	//}
	//// ��������ʱ��
	//mDestoryTime = mExpiredTime + INSTANCE_DESTORY_TIME;

	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "Instance_EndFunc", mType, mInstanceID);
}

/// ����
void CInstance::destroy()
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "Instance_DestroyFunc", mType, mInstanceID);
}

/// ����
void CInstance::givePrize(CEntityPlayer& rPlayer)
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_GivePrizeFunc", mType, mInstanceID, &rPlayer);
}

/// ��ҽ���
void CInstance::playerEnter(CEntityPlayer& rPlayer)
{
	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_PlayerEnterFunc", mType, mInstanceID, &rPlayer);
}

/// �¼�����
void CInstance::onEvent(EmInstanceEvent eEventType, int nParam1, int nParam2)
{
	//// ���������ĸ��¼�
	//if (!mCareEvent.getBit(eEventType))
	//{
	//	return;
	//}

	//lua_State* L = CSceneJob::Inst()->getLuaState();
	//lua_tinker::call<int>(L, "ServerActivity_OnEventFunc", mType, mInstanceID, eEventType, nParam1, nParam2);
}