#include "dailyactmodule.h"
/// ����������
void CDailyActModule::onLaunchServer()
{

}

/// ������ɼ��
bool CDailyActModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CDailyActModule::onLaunchComplete()
{

}

/// �˳�������
void CDailyActModule::onExitServer()
{

}

/// ��һ�쵽��
void CDailyActModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CDailyActModule::onNewWeekCome()
{

}

/// ����ʵ��
void CDailyActModule::onCreatePlayer(CEntity* pEntity)
{

}

/// ����ʵ��
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