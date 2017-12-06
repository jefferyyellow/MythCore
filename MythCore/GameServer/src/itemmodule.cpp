#include "itemmodule.hxx.pb.h"
#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
void CItemModule::OnTimer(unsigned int nTickOffset)
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
			pPlayer->GetItemUnit().onUseItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_SELL_ITEM:
		{
			pPlayer->GetItemUnit().onSellItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_PURCHASE_ITEM:
		{
			pPlayer->GetItemUnit().onPurchaseItemRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_EQUIP_ITEM:
		{
			//pPlayer->GetItemUnit()
			break;
		}
		case ID_C2S_REQUEST_UNEQUIP_ITEM:
		{
			break;
		}
	}
}

