#include "chatmodule.h"
CChatModule::CChatModule()
{
	
}

CChatModule::~CChatModule()
{
	
}

/// ����������
void CItemModule::onLaunchServer()
{

}

/// ������ɼ��
bool CItemModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CItemModule::onLaunchComplete()
{

}

/// �˳�������
void CItemModule::onExitServer()
{

}

/// ��һ�쵽��
void CItemModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CItemModule::onNewWeekCome()
{

}

/// ����ʵ��
void CItemModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
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