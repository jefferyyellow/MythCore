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

/// 启动服务器
void CChatModule::onLaunchServer()
{

}

/// 启动完成检查
bool CChatModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CChatModule::onLaunchComplete()
{

}

/// 退出服务器
void CChatModule::onExitServer()
{

}

/// 新一天到来
void CChatModule::onNewDayCome()
{

}

/// 新一周到来
void CChatModule::onNewWeekCome()
{

}

/// 建立实体
void CChatModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// 销毁实体
void CChatModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

void CChatModule::onTimer(unsigned int nTickOffset)
{

}

/// 加载配置文件
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
	// 已经禁言
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

/// 发送世界新闻
void CChatModule::sendWorldNews(CEntityPlayer& rPlayer, EmWorldNewsType eNewsType, PBNewsParam& rNewParam)
{
	CWorldNewsNotify tNotify;
	tNotify.set_type(eNewsType);
	createPBNewsRole(rPlayer, tNotify.mutable_role());
	tNotify.mutable_newsparam()->CopyFrom(rNewParam);
	
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_WORLD_NEWS, &tNotify);
}

/// 发送世界新闻
void CChatModule::sendWorldNews(EmWorldNewsType eNewsType, PBNewsParam& rNewParam)
{
	CWorldNewsNotify tNotify;
	tNotify.set_type(eNewsType);
	tNotify.mutable_newsparam()->CopyFrom(rNewParam);
	CSceneJob::Inst()->send2AllPlayer(ID_S2C_NOTIFY_WORLD_NEWS, &tNotify);
}

/// 创建世界传闻的玩家信息
void CChatModule::createPBNewsRole(CEntityPlayer& rPlayer, PBNewsRole* pbNewRole)
{
	if (NULL == pbNewRole)
	{
		return;
	}

	pbNewRole->set_playerid(rPlayer.getRoleID());
	pbNewRole->set_playername(rPlayer.getName());
}
