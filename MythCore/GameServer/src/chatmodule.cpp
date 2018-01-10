#include "chatmodule.h"
CChatModule::CChatModule()
{
	
}

CChatModule::~CChatModule()
{
	
}

/// 启动服务器
void CItemModule::onLaunchServer()
{

}

/// 启动完成检查
bool CItemModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CItemModule::onLaunchComplete()
{

}

/// 退出服务器
void CItemModule::onExitServer()
{

}

/// 新一天到来
void CItemModule::onNewDayCome()
{

}

/// 新一周到来
void CItemModule::onNewWeekCome()
{

}

/// 建立实体
void CItemModule::onCreatePlayer(CEntity* pEntity)
{

}

/// 销毁实体
void CItemModule::onDestroyPlayer(CEntity* pEntity)
{

}

void CItemModule::onTimer(unsigned int nTickOffset)
{

}

void CItemModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch(nMessageID)
	{
		case ID_C2S_REQUEST_CHAT:
		{
			break;
		}
	}
}