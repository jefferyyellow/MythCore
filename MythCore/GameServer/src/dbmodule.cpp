#include "dbmodule.h"
#include "gameserver.h"
#include "propertymodule.h"
void CDBModule::OnTimer(unsigned int nTickOffset)
{

}

/// 压入DB任务
void CDBModule::pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, char* pSql, ...)
{
	va_list tArgs;
	va_start(tArgs, pSql);
	// +1表示吧终止符也拷贝过去
	int nLength = ::vsnprintf((char*)mDBRequest.mSqlBuffer, sizeof(mDBRequest.mSqlBuffer) - 1, pSql, tArgs) + 1;
	va_end(tArgs);

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
	}
}

