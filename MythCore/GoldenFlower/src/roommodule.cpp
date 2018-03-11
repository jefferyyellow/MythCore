#include "roommodule.h"
#include "roommodule.hxx.pb.h"
#include "locallogjob.h"
#include "room.h"
#include "objpool.h"
#include "entityplayer.h"
#include "scenejob.h"
/// ����������
void CRoomModule::onLaunchServer()
{

}

/// ������ɼ��
bool CRoomModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CRoomModule::onLaunchComplete()
{

}

/// �˳�������
void CRoomModule::onExitServer()
{

}

/// ��һ�쵽��
void CRoomModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CRoomModule::onNewWeekCome()
{

}

/// ����ʵ��
void CRoomModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CRoomModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CRoomModule::onTimer(unsigned int nTickOffset)
{

}

void CRoomModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_CREATE_ROOM:
		{
			onCreateRoom(pPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_ENTER_GAME:
		{
			onEnterGame(pPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_READY:
		{
			onReady(pPlayer, pMessage);
			break;
		}
		case ID_C2S_REQUEST_LEAVE_ROOM:
		{
			onLeaveRoom(pPlayer, pMessage);
			break;
		}
	}
}

void CRoomModule::onCreateRoom(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage ,return);
	UCreateGame* pCreateRoom = static_cast<UCreateGame*>(pMessage);
	if (NULL == pCreateRoom)
	{
		return;
	}

	CRoom* pRoom = static_cast<CRoom*>(CObjPool::Inst()->allocObj(emObjType_Room));
	if (NULL != pRoom)
	{
		return;
	}

	pRoom->init();
	pRoom->addPlayer(pPlayer);
	pPlayer->setRoomID(pRoom->getObjID());
}

void CRoomModule::onEnterGame(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	UEnterGame* pEnterGame = static_cast<UEnterGame*>(pMessage);
	if (NULL == pEnterGame)
	{
		return;
	}

	int nRoomID = pEnterGame->roomid();
	int nRoomObjID = 0;
	CRoom* pRoom = static_cast<CRoom*>(CObjPool::Inst()->getObj(nRoomObjID));
	if (NULL == pRoom)
	{
		return;
	}

	pRoom->addPlayer(pPlayer);
	pPlayer->setRoomID(pRoom->getObjID());
}

void CRoomModule::onLeaveRoom(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CRoom* pRoom = static_cast<CRoom*>(CObjPool::Inst()->getObj(pPlayer->getObjID()));
	if (NULL == pRoom)
	{
		pPlayer->clear();
		return;
	}

	pRoom->removePlayer(pPlayer);
	pPlayer->clear();
}


void CRoomModule::onReady(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CRoom* pRoom = static_cast<CRoom*>(CObjPool::Inst()->getObj(pPlayer->getObjID()));
	if (NULL == pRoom)
	{
		return;
	}

	pPlayer->setReady(true);
	pRoom->onPlayerReady(pPlayer);
}

void CRoomModule::onCheckPoker(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

	DYSZCheckPoker tCheckPoker;
	tCheckPoker.set_card1(pPlayer->getPoker(0));
	tCheckPoker.set_card2(pPlayer->getPoker(1));
	tCheckPoker.set_card3(pPlayer->getPoker(2));
	CSceneJob::Inst()->send2Player(pPlayer, ID_S2C_RESPONSE_CHECK_POKER, &tCheckPoker);
}
