#include "instancemodule.h"
/// 启动服务器
void CInstanceModule::onLaunchServer()
{

}

/// 启动完成检查
bool CInstanceModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CInstanceModule::onLaunchComplete()
{

}

/// 退出服务器
void CInstanceModule::onExitServer()
{

}

/// 新一天到来
void CInstanceModule::onNewDayCome()
{

}

/// 新一周到来
void CInstanceModule::onNewWeekCome()
{

}

/// 建立实体
void CInstanceModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CInstanceModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CInstanceModule::onTimer(unsigned int nTickOffset)
{

}

void CInstanceModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	 
}

CInstance* CInstanceModule::createInstance(int nInstance)
{
	
	return NULL;
}