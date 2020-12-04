#include "guildmodule.h"
#include "entityplayer.h"

CGuildModule::CGuildModule()
{

}

CGuildModule::~CGuildModule()
{

}

/// 启动服务器
void CGuildModule::onLaunchServer()
{

}

/// 启动完成检查
bool CGuildModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CGuildModule::onLaunchComplete()
{

}

/// 退出服务器
void CGuildModule::onExitServer()
{

}

/// 新一天到来
void CGuildModule::onNewDayCome()
{

}

/// 新一周到来
void CGuildModule::onNewWeekCome()
{

}

/// 建立实体
void CGuildModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// 销毁实体
void CGuildModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

/// 时间函数
void CGuildModule::onTimer(unsigned int nTickOffset)
{

}

/// 加载配置文件
void CGuildModule::onLoadConfig()
{

}