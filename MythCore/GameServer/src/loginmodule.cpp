#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "dbmessage.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "loginplayer.h"
#include "dbmodule.h"
#include "scenejob.h"
#include "propertymodule.h"
#include "timemanager.h"
#include "errcode.h"
CLoginModule::CLoginModule()
:mLoginCheckTime(1000)
{
	init();
}


CLoginModule::~CLoginModule()
{

}

void CLoginModule::init()
{
    memset(mAllocateRoleId, 0, sizeof(mAllocateRoleId));
    mCheckResult = false;
}

/// ����������
void CLoginModule::onLaunchServer()
{
	loadAllocateRoleId();
}

/// ������ɼ��
bool CLoginModule::onCheckLaunch()
{
	return mCheckResult;
}

/// �����������ɹ�
void CLoginModule::onLaunchComplete()
{

}

/// �˳�������
void CLoginModule::onExitServer()
{

}

/// ��һ�쵽��
void CLoginModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CLoginModule::onNewWeekCome()
{

}

/// ����ʵ��
void CLoginModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CLoginModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

/// ʱ�亯��
void CLoginModule::onTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CTimeManager::Inst()->getCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		LOGIN_LIST::iterator itTmp = mLoginList.end();
		for (; it != mLoginList.end(); )
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				itTmp = it;
				++ it;
				mLoginList.erase(itTmp);
				continue;
			}

			bool bOverTime = pLoginPlayer->elapse(1);
			// �����ɾ��״̬���߳�ʱ
			if (emLoginDelState_None != pLoginPlayer->getDelState() || bOverTime)
			{
				// ����Ѿ��ڼ���״̬�ˣ��ǿ϶��ж�Ӧ��CEntityPlayer�����ܴ��̣�ֱ��ɾ����Ӧ��ʵ���socket
				if (emLoginState_RoleLoading == pLoginPlayer->getLoginState())
				{
					CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerBySocketIndex(pLoginPlayer->getExchangeHead().mSocketIndex);
					if (NULL != pPlayer)
					{
						CPropertyModule::Inst()->playerLeaveGame(pPlayer, EmLeaveReason_LoadTimeOut);
					}
				}
				printf("delete login player, State: %d, DelState: %d, OverTime: %d", pLoginPlayer->getLoginState(), pLoginPlayer->getDelState(), bOverTime);
				removeVerifyPlayer(pLoginPlayer->getChannelID(), pLoginPlayer->getServerID(), pLoginPlayer->getAccountID());
				
				itTmp = it;
				++it;
				mLoginList.erase(itTmp);

				CObjPool::Inst()->free(nObjID);
				// �����¼����˾Ͳ��ܶϿ�����
				if (emLoginDelState_Complete != pLoginPlayer->getDelState())
				{
					CSceneJob::Inst()->disconnectPlayer(pLoginPlayer->getExchangeHead());
				}
				continue;
			}

			++ it;
		}
	}
}

/// ���������ļ�
void CLoginModule::onLoadConfig()
{

}

void CLoginModule::onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pMessage)
	{
		return;
	}
	CLoginPlayer* pLoginPlayer = NULL;
	LOGIN_LIST::iterator it = mLoginList.find(rExchangeHead.mSocketIndex);
	if (it == mLoginList.end())
	{
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
		// ����ʧ��
		if (NULL == pLoginPlayer)
		{
			return;
		}
		std::pair<LOGIN_LIST::iterator, bool> ret = mLoginList.insert(LOGIN_LIST::value_type(rExchangeHead.mSocketIndex, pLoginPlayer->getObjID()));
		// �����б�ʧ��
		if (!ret.second)
		{
			CObjPool::Inst()->free(pLoginPlayer->getObjID());
			return;
		}
		pLoginPlayer->getExchangeHead() = rExchangeHead;
	}
	else
	{
		pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	if (NULL == pLoginPlayer)
	{
		return;
	}

	printf("pLoginPlayer: %d", pLoginPlayer->getObjID());

	// �����ɾ��״̬�ˣ�ֱ�Ӳ�����
	if (emLoginDelState_None != pLoginPlayer->getDelState())
	{
		return;
	}

	pLoginPlayer->setClientMessage(pMessage);
	pLoginPlayer->setClientMessageID(nMessageID);
	pLoginPlayer->checkState();
	pLoginPlayer->setClientMessage(NULL);
	pLoginPlayer->setClientMessageID(0);

	const ::google::protobuf::Descriptor* pDescriptor = pMessage->GetDescriptor();

	LOG_DEBUG("default", "---- Receive from client(Obj Id:%d|Account Id:%d|Channel:%d|Server:%d|Role:%d|Name:%s) \
Msg[ %s ][id: 0x%04x/%d] ---",
		pLoginPlayer->getObjID(), pLoginPlayer->getAccountID(), pLoginPlayer->getChannelID(),
		pLoginPlayer->getServerID(), pLoginPlayer->getRoleID(), pLoginPlayer->getAccountName(), 
		pDescriptor->name().c_str(),nMessageID, nMessageID);
	LOG_DEBUG("default", "[%s]", pMessage->ShortDebugString().c_str());
}

void CLoginModule::OnDBMessage(CDBResponse* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	if (pMsg->mSqlResult != SUCCESS)
	{
		return;
	}

	int nLoginPlayerObjID  = pMsg->mParam1;
	CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nLoginPlayerObjID));
	if (NULL == pLoginPlayer)
	{
		return;
	}

	// �����ɾ��״̬�ˣ�ֱ�Ӳ�����
	if (emLoginDelState_None != pLoginPlayer->getDelState())
	{
		return;
	}

	printf("OnDBMessage: %d, %s\n", nLoginPlayerObjID, pLoginPlayer->getAccountName());
	pLoginPlayer->setDBResponse(pMsg);
	pLoginPlayer->setDBSessionType((EmSessionType)pMsg->mSessionType);
	pLoginPlayer->checkState();
	pLoginPlayer->setDBResponse(NULL);
	pLoginPlayer->setDBSessionType(emSessionType_None);
}

/// ����ȴ���ҽ�����Ϸ
void CLoginModule::processWaitEnterGame(CLoginPlayer* pLoginPlayer, Message* pMessage)
{
	if (NULL == pLoginPlayer || NULL == pMessage)
	{
		return;
	}
	if (pLoginPlayer->getLoginState() != emLoginState_LoginComplete)
	{
		return;
	}
	CEnterSceneRequest* pEnterSceneRequest = static_cast<CEnterSceneRequest*>(pMessage);
	if (NULL == pEnterSceneRequest)
	{
		return;
	}


}

///  һ��Socket�Ͽ�
void CLoginModule::onSocketDisconnect(int nSocketIndex)
{
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it != mLoginList.end())
	{
		CLoginPlayer* pLoginPlayer = static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
		if (NULL != pLoginPlayer)
		{
			removeVerifyPlayer(pLoginPlayer->getChannelID(), pLoginPlayer->getServerID(), pLoginPlayer->getAccountID());
		}
		// ���ʹ�����ͷŵ�
		CObjPool::Inst()->free(it->second);
		mLoginList.erase(nSocketIndex);
	}
}

/// �������ݿ�������ɫID���ֶ�
void CLoginModule::loadAllocateRoleId()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0,"select * from AllocateRoleId order by server_id asc limit %d", BAT_LOAD_ROLE_ID_NUM);
}

/// �������ݿ�������ɫID�Ĵ���
void CLoginModule::onLoadAllocateRoleId(CDBResponse& rResponse)
{
	int nRow = rResponse.mRowNum;
	int nMaxServerId = 0;
	for (int i = 0; i < nRow; ++ i)
	{
		int nServerID = rResponse.getInt();
		unsigned int nRoleID = rResponse.getInt();
		setAllocateRoleId(nServerID, nRoleID);
		nMaxServerId = nServerID;
	}

	// ��û�������
	if (nRow >= BAT_LOAD_ROLE_ID_NUM)
	{
		CDBModule::Inst()->pushDBTask(0, emSessionType_LoadAllocateRoleId, 0, 0, "select * from AllocateRoleId where server_id > %d order by server_id asc limit %d", nMaxServerId, BAT_LOAD_ROLE_ID_NUM);
	}
	// �Ѿ���������
	else
	{
		int nMaxRoleID = getAllocateRoleId(CGameServer::Inst()->getServerID());
		if (0 == nMaxRoleID)
		{
			int nBeginRoleID = getBeginRoleID(CGameServer::Inst()->getServerID());
			if (nBeginRoleID == 0)
			{
				LOG_ERROR("begin role id is zero, server id: %d", CGameServer::Inst()->getServerID());
			}
			else
			{
				setAllocateRoleId(CGameServer::Inst()->getServerID(), nBeginRoleID);
				CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateAllocateRoleId, 0, 0, 
				"insert into AllocateRoleId (server_id,max_role_id) values(%d, %d)", CGameServer::Inst()->getServerID(), nBeginRoleID);
			}
		}
	}
	mCheckResult = true;
}

unsigned int CLoginModule::allocateRoleID(int nServerId)
{
	uint nMaxRoleId = getAllocateRoleId(nServerId);
	if(0 == nMaxRoleId)
	{
		return 0;
	}

	if (nMaxRoleId >= MAX_SERVER_ID * (uint)nServerId)
	{
		return 0;
	}

	setAllocateRoleId(nServerId, nMaxRoleId + 1);
	CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateAllocateRoleId, 0, 0,
		"update AllocateRoleId set max_role_id=%d where server_id=%d", nMaxRoleId + 1, nServerId);
	return nMaxRoleId;
}

/// ��������У������
bool CLoginModule::addVerifyPlayer(short nChannelId, short nServerId, int nAccountId, int nLoginObjId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	std::pair<VERIFY_LIST::iterator, bool> tRet = mVerifyList.insert(
		VERIFY_LIST::value_type(nLoginKey, nLoginObjId));
	if (tRet.second)
	{
		return true;
	}
	return false;
}

/// �Ƿ�������У������
bool CLoginModule::checkVerifyPlayer(short nChannelId, short nServerId, int nAccountId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	VERIFY_LIST::iterator it = mVerifyList.find(nLoginKey);
	if (it != mVerifyList.end())
	{
		return true;
	}

	return false;
}

/// ɾ������У������
void CLoginModule::removeVerifyPlayer(short nChannelId, short nServerId, int nAccountId)
{
	uint64 nLoginKey = MAKE_LOGIN_KEY((uint64)nAccountId, (uint64)nChannelId, (uint64)nServerId);
	mVerifyList.erase(nLoginKey);
}

/// �õ���¼�������
CLoginPlayer* CLoginModule::getLoginPlayer(int nSocketIndex)
{
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it != mLoginList.end())
	{
		return static_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	return NULL;
}