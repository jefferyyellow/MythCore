#include "roommodule.h"
/// 启动服务器
void CRoomModule::onLaunchServer()
{

}

/// 启动完成检查
bool CRoomModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CRoomModule::onLaunchComplete()
{

}

/// 退出服务器
void CRoomModule::onExitServer()
{

}

/// 新一天到来
void CRoomModule::onNewDayCome()
{

}

/// 新一周到来
void CRoomModule::onNewWeekCome()
{

}

/// 建立实体
void CRoomModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
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
	}
}