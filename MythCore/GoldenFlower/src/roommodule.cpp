#include "roommodule.h"
/// ����������
void CRoomModule::onLaunchServer()
{

}

/// ������ɼ��
bool CRoomModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CRoomModule::onLaunchComplete()
{

}

/// �˳�������
void CRoomModule::onExitServer()
{

}

/// ��һ�쵽��
void CRoomModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CRoomModule::onNewWeekCome()
{

}

/// ����ʵ��
void CRoomModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
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