#include "dbmodule.h"
#include "gameserver.h"
#include "propertymodule.h"
#include "objpool.h"
#include "entityplayer.h"
#include "jobmanager.h"
#include "platmodule.h"
#include "mailmodule.h"
#include "internalmsg.h"
/// ����������
void CDBModule::onLaunchServer()
{

}

/// ������ɼ��
bool CDBModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CDBModule::onLaunchComplete()
{

}

/// �˳�������
void CDBModule::onExitServer()
{

}

/// ��һ�쵽��
void CDBModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CDBModule::onNewWeekCome()
{

}

/// ����ʵ��
void CDBModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// ����ʵ��
void CDBModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

void CDBModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
void CDBModule::onLoadConfig()
{

}

/// ѹ��DB����
void CDBModule::pushDBTask(int nJobID, int nPlayerID, int nSessionType, int nParam1, int nParam2, const char* pSql, ...)
{
	CIMDBSqlRequest* pSqlRequest = new CIMDBSqlRequest;
	if (NULL == pSqlRequest)
	{
		return;
	}
	pSqlRequest->setMsgID(IM_REQUEST_DB_SQL);

	va_list tArgs;
	va_start(tArgs, pSql);
	// +1��ʾ����ֹ��Ҳ������ȥ
	int nLength = ::vsnprintf((char*)pSqlRequest->mSql, sizeof(pSqlRequest->mSql) - 1, pSql, tArgs) + 1;
	va_end(tArgs);
	printf("\n%s\n", pSqlRequest->mSql);
	CDBRequestHeader& rHeader = pSqlRequest->mHeader;
	rHeader.mJobID = nJobID;
	rHeader.mPlayerID = nPlayerID;
	rHeader.mParam1 = nParam1;
	rHeader.mParam2 = nParam2;
	rHeader.mSessionType = nSessionType;
	rHeader.mSqlLenth = nLength;

	CJobManager::Inst()->pushDBTask(nPlayerID, pSqlRequest);
}

void CDBModule::pushDBTask(int nJobID, int nPlayerID, int nSessionType, int nParam1, int nParam2, Message* pMessage)
{
	CIMDBMsgRequest* pMsgRequest = new CIMDBMsgRequest;
	if (NULL == pMsgRequest)
	{
		return;
	}
	pMsgRequest->setMsgID(IM_REQUEST_DB_MSG);
	CDBRequestHeader& rHeader = pMsgRequest->mHeader;
	rHeader.mJobID = nJobID;
	rHeader.mPlayerID = nPlayerID;
	rHeader.mParam1 = nParam1;
	rHeader.mParam2 = nParam2;
	rHeader.mSessionType = nSessionType;
	rHeader.mSqlLenth = pMessage->ByteSize();
	pMsgRequest->mSqlMsg = pMessage;

	CJobManager::Inst()->pushDBTask(nPlayerID, pMsgRequest);
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
				// ����ʧ��
				break;
			}
			CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(mDBResponse.mParam1));
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatusBit(emSaveStatus_Info);
				CPropertyModule::Inst()->onSavePlayerComplete(*pPlayer);
				printf("SavePlayerInfo complete\n");
			}
			break;
		}
		case emSessionType_SavePlayerBaseProperty:
		{
			if (mDBResponse.mSqlResult != 0)
			{
				// ����ʧ��
				break;
			}
			CEntityPlayer* pPlayer = static_cast<CEntityPlayer*>(CObjPool::Inst()->getObj(mDBResponse.mParam1));
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatusBit(emSaveStatus_BaseProperty);
				CPropertyModule::Inst()->onSavePlayerComplete(*pPlayer);
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

