#include "itemmodule.hxx.pb.h"
#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
#include "servercommon.h"

CItemModule::CItemModule()
{
	
}
CItemModule::~CItemModule()
{

}
/// 启动服务器
void CItemModule::onLaunchServer()
{
	CShopConfig tCommonShop(emShopType_Common);
	tCommonShop.loadShopConfig("gameserverconfig/shop/shop_common.xml");
	mShopList.push_back(tCommonShop);

	CShopConfig tLevelShop(emShopType_Level);
	tLevelShop.loadShopConfig("gameserverconfig/shop/shop_level.xml");
	mShopList.push_back(tLevelShop);
}

/// 启动完成检查
bool CItemModule::onCheckLaunch()
{
	return true;
}

/// 服务器启动成功
void CItemModule::onLaunchComplete()
{

}

/// 退出服务器
void CItemModule::onExitServer()
{

}

/// 新一天到来
void CItemModule::onNewDayCome()
{

}

/// 新一周到来
void CItemModule::onNewWeekCome()
{

}

/// 建立实体
void CItemModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// 销毁实体
void CItemModule::onDestroyPlayer(CEntityPlayer* pPlayer)
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
		case ID_C2S_REQUEST_GET_SHOP_INFO:
		{
			pPlayer->getItemUnit().onGetShopInfoRequest(pMessage);
			break;
		}
	}
}

int	CItemModule::getFromDropTable(int nDropTableID, int& rDropItemNum)
{
	// 得到具体的掉落表
	CTemplate* tpTempalte = CStaticData::searchTpl(nDropTableID);
	if (NULL == tpTempalte)
	{
		return 0;
	}

	if (tpTempalte->mTemplateType != emTemplateType_DropTable)
	{
		return 0;
	}
	CTplDropTable* pTplDropTable = static_cast<CTplDropTable*>(tpTempalte);

	int nRandom = RAND(MYTH_PERCENT_INT);
	// 得到掉落ID
	for (int i = 0; i < MAX_DROP_ITEM; i++)
	{
		if (pTplDropTable->mDropItem[i].mItemID == 0)
		{
			break;
		}
		if (nRandom >= pTplDropTable->mDropItem[i].mProbability)
		{
			continue;
		}

		rDropItemNum = pTplDropTable->mDropItem[i].mItemNum;
		return pTplDropTable->mDropItem[i].mItemID;
	}
	return 0;
}