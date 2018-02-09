#include "dailyactmodule.h"
/// 启动服务器
void CDailyActModule::onLaunchServer()
{

}

/// 启动完成检查
bool CDailyActModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CDailyActModule::onLaunchComplete()
{

}

/// 退出服务器
void CDailyActModule::onExitServer()
{

}

/// 新一天到来
void CDailyActModule::onNewDayCome()
{

}

/// 新一周到来
void CDailyActModule::onNewWeekCome()
{

}

/// 建立实体
void CDailyActModule::onCreatePlayer(CEntity* pEntity)
{

}

/// 销毁实体
void CDailyActModule::onDestroyPlayer(CEntity* pEntity)
{

}

void CDailyActModule::onTimer(unsigned int nTickOffset)
{

}

void CDailyActModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{

	}
}