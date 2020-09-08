#include "instancemodule.h"
#include "instance.h"
#include "objpool.h"
/// ����������
void CInstanceModule::onLaunchServer()
{

}

/// ������ɼ��
bool CInstanceModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CInstanceModule::onLaunchComplete()
{

}

/// �˳�������
void CInstanceModule::onExitServer()
{

}

/// ��һ�쵽��
void CInstanceModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CInstanceModule::onNewWeekCome()
{

}

/// ����ʵ��
void CInstanceModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CInstanceModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CInstanceModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
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