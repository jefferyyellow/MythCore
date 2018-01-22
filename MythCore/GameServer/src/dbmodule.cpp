#include "dbmodule.h"
#include "gameserver.h"
#include "propertymodule.h"
#include "objpool.h"
#include "entityplayer.h"

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
void CDBModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
void CDBModule::onDestroyPlayer(CEntity* pEntity)
{

}

void CDBModule::onTimer(unsigned int nTickOffset)
{

}

/// ѹ��DB����
void CDBModule::pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, const char* pSql, ...)
{
	va_list tArgs;
	va_start(tArgs, pSql);
	// +1��ʾ����ֹ��Ҳ������ȥ
	int nLength = ::vsnprintf((char*)mDBRequest.mSqlBuffer, sizeof(mDBRequest.mSqlBuffer) - 1, pSql, tArgs) + 1;
	va_end(tArgs);
	printf("\n%s\n", mDBRequest.mSqlBuffer);
	mDBRequest.mPlayerID = nPlayerID;
	mDBRequest.mParam1 = nParam1;
	mDBRequest.mParam2 = nParam2;
	mDBRequest.mSessionType = nSessionType;
	mDBRequest.mSqlLenth = nLength;

	int nTotalLength = nLength + sizeof(CDBRequestHeader);
	CGameServer::Inst()->pushDBTask(nPlayerID, (byte*)(&mDBRequest), nTotalLength);
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
	CGameServer::Inst()->pushDBTask(nPlayerID, (byte*)(&mDBRequest), nTotalLength);
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
			if (mDBResponse.mResult != 0)
			{
				// ����ʧ��
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
			if (mDBResponse.mResult != 0)
			{
				// ����ʧ��
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
	}
}

