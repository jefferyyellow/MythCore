#include "loginjob.h"
#include "internalmsgpool.h"
#include "objpool.h"
#include "gameserver.h"
void CLoginJob::doing(uint32 uParam)
{
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// Èç¹û
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
}

void CLoginJob::onTask(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}

	switch (pMsg->getMsgID())
	{
		case IM_RESPONSE_PLAYER_LOGIN:
		{
			CIMPlayerLoginResponse* pResponse = reinterpret_cast<CIMPlayerLoginResponse*>(pMsg);

			CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::CreateInst()->allocObj(emObjType_LoginPlayer));
			if (NULL == pLoginPlayer)
			{
				return;
			}
			uint64 nAccountID = pResponse->mAccountID;
			uint64 nChannelID = pResponse->mChannelID;
			uint64 nWorldID = pResponse->mWorldID;
			pLoginPlayer->setKey(MAKE_LOGIN_KEY(nAccountID, nChannelID, nWorldID));
			pLoginPlayer->setSocketIndex(pResponse->mSocketIndex);
			pLoginPlayer->setSocketTime(pResponse->mSocketTime);

			mLoginList.Insert(pLoginPlayer->getKey(), pLoginPlayer->getObjID());

			CIMPlayerLoginResponse* pNewResponse = reinterpret_cast<CIMPlayerLoginResponse*>(CInternalMsgPool::CreateInst()->allocMsg(IM_RESPONSE_PLAYER_LOGIN));
			if (NULL == pNewResponse)
			{
				return;
			}
			pNewResponse->mAccountID = pResponse->mAccountID;
			pNewResponse->mChannelID = pResponse->mChannelID;
			pNewResponse->mWorldID = pResponse->mWorldID;
			pNewResponse->mRoleID = pResponse->mRoleID;
			pNewResponse->mSocketIndex = pResponse->mSocketIndex;
			pNewResponse->mSocketTime = pResponse->mSocketTime;

			CGameServer::Inst()->pushTask(emTaskType_Scene, pNewResponse);
			break;
		}
		default:
			break;
	}
}
