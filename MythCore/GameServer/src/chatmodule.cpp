#include "chatmodule.h"
#include "chatmodule.hxx.pb.h"
#include "locallogjob.h"
#include "entityplayer.h"
#include "scenejob.h"
#include "timemanager.h"
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
void CChatModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// ����ʵ��
void CChatModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

void CChatModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
void CChatModule::onLoadConfig()
{

}

void CChatModule::onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch(nMessageID)
	{
		case ID_C2S_REQUEST_CHAT:
		{
			onChatRequest(rPlayer, pMessage);
			break;
		}
	}
}

void CChatModule::onChatRequest(CEntityPlayer& rPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CChatRequest* pChatRequest = static_cast<CChatRequest*>(pMessage);
	CTplChatConfig* pTplChatConfig = CTplChatConfig::spChatConfig;
	if (NULL == pTplChatConfig)
	{
		return;
	}
	int nChannel = pChatRequest->channel();
	// �Ѿ�����
	if (rPlayer.getAccountStatus(emAccountStatus_Mute))
	{
		return;
	}

	CChatNotify tChatNotify;
	tChatNotify.set_playerid(rPlayer.getRoleID());
	tChatNotify.set_playername(rPlayer.getName());
	tChatNotify.set_channel(nChannel);
	tChatNotify.set_content(pChatRequest->content());


	time_t tNowTime = CTimeManager::Inst()->getCurrTime();
	switch(nChannel)
	{
		case emChatChannel_World:
		{
			CTplChatConfig::CChatChannelLimit& rLimit = pTplChatConfig->mWorldLimit;
			if (pChatRequest->content().length() > (uint)rLimit.mWordNum * 3)
			{
				return;
			}

			if (rPlayer.getLevel() < rLimit.mPlayerLevel)
			{
				return;
			}

			if (tNowTime - rPlayer.getInteractiveUnit().getChatTime(emChatChannel_World) < rLimit.mInterval)
			{
				return;
			}

			CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_CHAT, &tChatNotify);
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

/// ������������
void CChatModule::sendWorldNews(CEntityPlayer& rPlayer, EmWorldNewsType eNewsType, PBNewsParam& rNewParam)
{
	CWorldNewsNotify tNotify;
	tNotify.set_type(eNewsType);
	createPBNewsRole(rPlayer, tNotify.mutable_role());
	tNotify.mutable_newsparam()->CopyFrom(rNewParam);
	
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_WORLD_NEWS, &tNotify);
}

/// ������������
void CChatModule::sendWorldNews(EmWorldNewsType eNewsType, PBNewsParam& rNewParam)
{
	CWorldNewsNotify tNotify;
	tNotify.set_type(eNewsType);
	tNotify.mutable_newsparam()->CopyFrom(rNewParam);
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_WORLD_NEWS, &tNotify);
}

/// �������紫�ŵ������Ϣ
void CChatModule::createPBNewsRole(CEntityPlayer& rPlayer, PBNewsRole* pbNewRole)
{
	if (NULL == pbNewRole)
	{
		return;
	}

	pbNewRole->set_playerid(rPlayer.getRoleID());
	pbNewRole->set_playername(rPlayer.getName());
}
