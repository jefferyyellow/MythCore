#include "itemmodule.hxx.pb.h"
#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
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
		case ID_C2S_REQUEST_USE_ITEM:
		{
			pPlayer->getItemUnit().onUseItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_SELL_ITEM:
		{
			pPlayer->getItemUnit().onSellItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_PURCHASE_ITEM:
		{
			pPlayer->getItemUnit().onPurchaseItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_EQUIP_ITEM:
		{
			pPlayer->getItemUnit().onEquipItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_UNEQUIP_ITEM:
		{
			pPlayer->getItemUnit().onUnEquipItemRequest(pMessage);
			break;
		}
	}
}

