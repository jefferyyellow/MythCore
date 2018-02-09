#include "serveractmodule.h"

/// 启动服务器
void CServerActModule::onLaunchServer()
{

}

/// 启动完成检查
bool CServerActModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CServerActModule::onLaunchComplete()
{

}

/// 退出服务器
void CServerActModule::onExitServer()
{

}

/// 新一天到来
void CServerActModule::onNewDayCome()
{

}

/// 新一周到来
void CServerActModule::onNewWeekCome()
{

}

/// 建立实体
void CServerActModule::onCreatePlayer(CEntity* pEntity)
{

}

/// 销毁实体
void CServerActModule::onDestroyPlayer(CEntity* pEntity)
{

}

void CServerActModule::onTimer(unsigned int nTickOffset)
{

}

void CServerActModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{

	}
}