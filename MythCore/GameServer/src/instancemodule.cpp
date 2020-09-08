#include "instancemodule.h"
#include "instance.h"
#include "objpool.h"
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

/// 加载配置文件
void CInstanceModule::onLoadConfig()
{

}

void CInstanceModule::onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage)
{
	 
}

CInstance* CInstanceModule::createInstance(EmInstanceType eType, int nInstanceID)
{
	CInstance* pInstance = (CInstance*)CObjPool::Inst()->allocObj(emObjType_Instance);
	if (NULL == pInstance)
	{
		return NULL;
	}

	pInstance->setType(eType);
	
	return NULL;
}