#include "chatmodule.h"
#include "chatmodule.hxx.pb.h"
#include "locallogjob.h"
#include "entityplayer.h"
#include "scenejob.h"
CChatModule::CChatModule()
{
	init();
}

CChatModule::~CChatModule()
{
	
}

void CChatModule::init()
{
}

/// ����������
void CChatModule::onLaunchServer()
{

}

/// ������ɼ��
bool CChatModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CChatModule::onLaunchComplete()
{

}

/// �˳�������
void CChatModule::onExitServer()
{

}

/// ��һ�쵽��
void CChatModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CChatModule::onNewWeekCome()
{

}

/// ����ʵ��
void CChatModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CChatModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CChatModule::onTimer(unsigned int nTickOffset)
{

}

void CChatModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch(nMessageID)
	{
		case ID_C2S_REQUEST_CHAT:
		{
			onChatRequest(pPlayer, pMessage);
			break;
		}
	}
}

void CChatModule::onChatRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);
	CChatRequest* pChatRequest = static_cast<CChatRequest*>(pMessage);

	int nChannel = pChatRequest->channel();
	if(pChatRequest->content().length() > MAX_CHAT_CONTENT_LENG)
	{
		return;
	}

	CChatNotify tChatNotify;
	tChatNotify.set_playerid(pPlayer->getRoleID());
	tChatNotify.set_playername(pPlayer->getName());
	tChatNotify.set_channel(nChannel);
	tChatNotify.set_content(pChatRequest->content());

	switch(nChannel)
	{
		case emChatChannel_World:
		{
			CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_CHAT, pMessage);
			break;
		}
		case emChatChannel_Team:
		{
			break;
		}
		case emChatChannel_Faction:
		{
			break;
		}
	}
}