#include "loginmodule.h"
#include "loginmessage.hxx.pb.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "timemanager.h"
#include "loginplayer.h"
CLoginModule::CLoginModule()
:mLoginCheckTime(1000)
{

}


CLoginModule::~CLoginModule()
{

}

/// ʱ�亯��
void CLoginModule::OnTimer(unsigned int nTickOffset)
{
	if (mLoginCheckTime.elapse(nTickOffset))
	{
		time_t tTimeNow = CTimeManager::Inst()->GetCurrTime();
		LOGIN_LIST::iterator it = mLoginList.begin();
		for (; it != mLoginList.end(); )
		{
			int nObjID = it->second;
			CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(nObjID));
			if (NULL == pLoginPlayer)
			{
				it = mLoginList.erase(it);
				continue;
			}
			// ��ʱ
			if (pLoginPlayer->elapse(nTickOffset))
			{
				CObjPool::Inst()->free(nObjID);
				it = mLoginList.erase(it);
				continue;
			}

			++ it;
		}
	}
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
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->allocObj(emObjType_LoginPlayer));
		// ����ʧ��
		if (NULL == pLoginPlayer)
		{
			return;
		}
		std::pair<LOGIN_LIST::iterator,bool> ret = mLoginList.insert(LOGIN_LIST::value_type(rExchangeHead.mSocketIndex, pLoginPlayer->getObjID()));
		// �����б�ʧ��
		if (!ret.second)
		{
			CObjPool::Inst()->free(pLoginPlayer->getObjID());
			return;
		}
		pLoginPlayer->getExchangeHead() = rExchangeHead;
		it = ret.first;
	}
	else
	{
		pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	}
	if (NULL == pLoginPlayer)
	{
		return;
	}
	pLoginPlayer->setClientMessage(pMessage);
	pLoginPlayer->setClientMessageID(nMessageID);
	pLoginPlayer->checkState();
}

void CLoginModule::OnDBMessage(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	CIMPlayerLoginMsg* pPlayerLoginMsg = (CIMPlayerLoginMsg*)pMsg;
	int nSocketIndex = pPlayerLoginMsg->mSocketIndex;
	LOGIN_LIST::iterator it = mLoginList.find(nSocketIndex);
	if (it == mLoginList.end())
	{
		return;
	}

	CLoginPlayer* pLoginPlayer = reinterpret_cast<CLoginPlayer*>(CObjPool::Inst()->getObj(it->second));
	if (NULL == pLoginPlayer)
	{
		return;
	}
	pLoginPlayer->setDBMessage(pMsg);
	pLoginPlayer->setDBMessageID(pMsg->getMsgID());
	pLoginPlayer->checkState();
}