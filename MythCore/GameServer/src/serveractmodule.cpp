#include "serveractmodule.h"

/// ����������
void CServerActModule::onLaunchServer()
{

}

/// ������ɼ��
bool CServerActModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CServerActModule::onLaunchComplete()
{

}

/// �˳�������
void CServerActModule::onExitServer()
{

}

/// ��һ�쵽��
void CServerActModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CServerActModule::onNewWeekCome()
{

}

/// ����ʵ��
void CServerActModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
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