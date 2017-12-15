#include "dbmodule.h"
#include "gameserver.h"
#include "propertymodule.h"
#include "objpool.h"
#include "entityplayer.h"
void CDBModule::OnTimer(unsigned int nTickOffset)
{

}

/// Ñ¹ÈëDBÈÎÎñ
void CDBModule::pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, char* pSql, ...)
{
	va_list tArgs;
	va_start(tArgs, pSql);
	// +1±íÊ¾°ÉÖÕÖ¹·ûÒ²¿½±´¹ýÈ¥
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
				// ´æÅÌÊ§°Ü
				break;
			}
			CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(mDBResponse.mParam1);
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatus(emSaveStatus_Info);
				CPropertyModule::Inst()->onSavePlayerComplete(pPlayer);
			}
			break;
		}
		case emSessionType_SavePlayerBaseProperty:
		{
			if (mDBResponse.mResult != 0)
			{
				// ´æÅÌÊ§°Ü
				break;
			}
			CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(mDBResponse.mParam1);
			if (NULL != pPlayer)
			{
				pPlayer->setSaveStatus(emSaveStatus_BaseProperty);
				CPropertyModule::Inst()->onSavePlayerComplete(pPlayer);
			}
			break;
		}
	}
}

