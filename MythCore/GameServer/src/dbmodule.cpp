#include "dbmodule.h"
#include "gameserver.h"
#include "propertymodule.h"
#include "objpool.h"
#include "entityplayer.h"
#include "jobmanager.h"
#include "platmodule.h"
#include "mailmodule.h"
/// 启动服务器
void CDBModule::onLaunchServer()
{

}

/// 启动完成检查
bool CDBModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CDBModule::onLaunchComplete()
{

}

/// 退出服务器
void CDBModule::onExitServer()
{

}

/// 新一天到来
void CDBModule::onNewDayCome()
{

}

/// 新一周到来
void CDBModule::onNewWeekCome()
{

}

/// 建立实体
void CDBModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CDBModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CDBModule::onTimer(unsigned int nTickOffset)
{

}

/// 压入DB任务
void CDBModule::pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, const char* pSql, ...)
{
	va_list tArgs;
	va_start(tArgs, pSql);
	// +1表示吧终止符也拷贝过去
	int nLength = ::vsnprintf((char*)mDBRequest.mSqlBuffer, sizeof(mDBRequest.mSqlBuffer) - 1, pSql, tArgs) + 1;
	va_end(tArgs);
	printf("\n%s\n", mDBRequest.mSqlBuffer);
	mDBRequest.mPlayerID = nPlayerID;
	mDBRequest.mParam1 = nParam1;
	mDBRequest.mParam2 = nParam2;
	mDBRequest.mSessionType = nSessionType;
	mDBRequest.mSqlLenth = nLength;

	int nTotalLength = nLength + sizeof(CDBRequestHeader);
	CJobManager::Inst()->pushDBTask(nPlayerID, (byte*)(&mDBRequest), nTotalLength);
}

void CDBModule::pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, Message* pMessage)
{
	if (!pMessage->SerializeToArray(mDBRequest.mSqlBuffer, sizeof(mDBRequest.mSqlBuffer) - 1))
	{
		LOG_ERROR("PushDBTask Error, SerializeToArray Failure, Session Type: %d", nSessionType);
		return;
	}
	mDBRequest.mPlayerID = nPlayerID;
	mDBRequest.mParam1 = nParam1;
	mDBRequest.mParam2 = nParam2;
	mDBRequest.mSessionType = nSessionType;
	mDBRequest.mSqlLenth = pMessage->ByteSize();

	int nTotalLength = pMessage->ByteSize() + sizeof(CDBRequestHeader);
	CJobManager::Inst()->pushDBTask(nPlayerID, (byte*)(&mDBRequest), nTotalLength);
}

void CDBModule::onDBSession()
{
	mDBResponse.resetPoint();
	switch (mDBResponse.mSessionType)
	{
		case emSessionType_AccountVerify:
		case emSessionType_CreateRole:
		{
			CLoginModule::Inst()->OnDBMessage(&mDBResponse);
			break;
		}
		case emSessionType_LoadPlayerInfo:
		{
			CPropertyModule::Inst()->onLoadPlayerInfo(mDBResponse);
			break;
		}
		case emSessionType_LoadPlayerBaseProperty:
		{
			CPropertyModule::Inst()->onLoadPlayerBaseProperty(mDBResponse);
			break;
		}
		case emSessionType_SavePlayerInfo:
		{
			if (mDBResponse.mSqlResult != 0)
			{
				// 存盘失败
				break;
			}
			CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(mDBResponse.mParam1));
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatusBit(emSaveStatus_Info);
				CPropertyModule::Inst()->onSavePlayerComplete(pPlayer);
				printf("SavePlayerInfo complete\n");
			}
			break;
		}
		case emSessionType_SavePlayerBaseProperty:
		{
			if (mDBResponse.mSqlResult != 0)
			{
				// 存盘失败
				break;
			}
			CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(mDBResponse.mParam1));
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatusBit(emSaveStatus_BaseProperty);
				CPropertyModule::Inst()->onSavePlayerComplete(pPlayer);
				printf("SavePlayerBaseProperty complete\n");
			}
			break;
		}
		case emSessionType_LoadAllocateRoleId:
		{
			CLoginModule::Inst()->onLoadAllocateRoleId(mDBResponse);
			break;
		}
		case emSessionType_InsertRechargeCache:
		{
			CPlatModule::Inst()->onInsertRechargeCache(mDBResponse);
			break;
		}
		case emSessionType_LoadRechargeCache:
		{
			CPlatModule::Inst()->onLoadRechargeCache(mDBResponse);
			break;
		}
		case emSessionType_RechargeSuccess:
		{
			CPlatModule::Inst()->onRechargeSuccess(mDBResponse);
			break;
		}
		case emSessionType_LoadPlayerMail:
		{
			CMailModule::Inst()->onLoadPlayerMail(mDBResponse);
			break;
		}
		case emSessionType_LoadMaxMailID:
		{
			CMailModule::Inst()->onLoadMaxMailID(mDBResponse);
			break;
		}
		case emSessionType_LoadGlobalMail:
		{
			CMailModule::Inst()->onLoadGlobalMail(mDBResponse);
			break;
		}
		case emSessionType_LoadMaxGlobalMailID:
		{
			CMailModule::Inst()->onLoadMaxGlobalMailID(mDBResponse);
			break;
		}
		case emSessionType_LoadGameEvent:
		{
			CMailModule::Inst()->onLoadGameEvent(mDBResponse);
			break;
		}
	}
}

