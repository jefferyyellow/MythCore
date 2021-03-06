#include "itemunit.h"
#include "itemmodule.hxx.pb.h"
#include "errcode.h"
#include "itemobject.h"
#include "template.h"
#include "locallogjob.h"
#include "entityplayer.h"
#include "itemmodule.h"
void CItemArrange::addItem(int nID, int nNum)
{
	if (mCount >= BASE_BAG_CAPACITY)
	{
		return;
	}

	mID[mCount] = nID;
	mNum[mCount] = nNum;
	++mCount;
}

/// 获得货币
int CItemUnit::obtainCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum)
{
	if (eCurrencyType < 0 || eCurrencyType >= emCurrencyMax)
	{
		return ERR_PARAMETER_INVALID;
	}

	if (nCurrencyNum < 0)
	{
		return ERR_OBTAIN_DIAMOND_INVALID;
	}
	if (0 == nCurrencyNum)
	{
		return SUCCESS;
	}

	int nCurrentCurrencyNum = mCurrency[eCurrencyType] + nCurrencyNum;
	mCurrency[eCurrencyType] = nCurrentCurrencyNum > MAX_CURRENCY_NUM ? MAX_CURRENCY_NUM : nCurrentCurrencyNum;

	CCurrencyUpdateNotify tMsg;
	tMsg.set_currencytype(eCurrencyType);
	tMsg.set_currencynum(mCurrency[eCurrencyType]);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_CURRENCY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费货币
int CItemUnit::consumeCurrency(EmCurrencyType eCurrencyType, int nCurrencyNum)
{
	if (eCurrencyType < 0 || eCurrencyType >= emCurrencyMax)
	{
		return ERR_PARAMETER_INVALID;
	}

	if (nCurrencyNum < 0)
	{
		return ERR_PARAMETER_INVALID;
	}
	if (0 == nCurrencyNum)
	{
		return SUCCESS;
	}
	if (mCurrency[eCurrencyType] < nCurrencyNum)
	{
		return ERR_CURRENCY_NOT_ENOUGH;
	}

	mCurrency[eCurrencyType] = mCurrency[eCurrencyType] - nCurrencyNum;

	CCurrencyUpdateNotify tMsg;
	tMsg.set_currencytype(eCurrencyType);
	tMsg.set_currencynum(mCurrency[eCurrencyType]);
	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_CURRENCY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 包裹是否能装下这些道具
bool CItemUnit::checkItemSpace(int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pItemID || NULL == pItemNum)
	{
		return false;
	}

	int nItemID[MAX_CONTAINER_ITEM_NUM];
	int nItemNum[MAX_CONTAINER_ITEM_NUM];
	int nItemCount = 0;
	for (int i = 0; i < nSize; ++ i)
	{
		if (pItemID[i] > MAX_CURRENCY_ID)
		{
			nItemID[nItemCount] = pItemID[i];
			nItemNum[nItemCount]= pItemNum[i];
			++ nItemCount;
		}
	}
	if (nItemCount > 0)
	{
		return mBag.checkSpace(nItemID, nItemNum, nItemCount);
	}
	return true;
}

/// 插入道具
int CItemUnit::insertAllItem(int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pItemID || NULL == pItemNum)
	{
		return ERR_PARAMETER_INVALID;
	}
	
	for (int i = 0; i < nSize; ++ i)
	{
		if (0 == pItemID[i])
		{
			break;
		}
		if (pItemID[i] <= MAX_CURRENCY_ID)
		{
			obtainCurrency((EmCurrencyType)CURRENCY_ID_2_TYPE(pItemID[i]), pItemNum[i]);
		}
		else
		{
			insertItem(pItemID[i], pItemNum[i]);
		}
	}
	

	return SUCCESS;
}

/// 插入道具
int CItemUnit::insertItem(int nItemID, int nItemNum)
{
	if (nItemID <= MAX_CURRENCY_ID)
	{
		return ERR_PARAMETER_INVALID;
	}

	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;
	
	CTplItem* pTplITem = static_cast<CTplItem*>(CStaticData::searchTpl(nItemID));
	if (NULL == pTplITem)
	{
		return ERR_TEMPLATE_INVALID;
	}

	int nResult = mBag.insertItem(nItemID, nItemNum, nOutItemIndex, nOutItemNum, nOutSize);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	// 通用的道具没有特殊属性
	if (emItemType_Equip == pTplITem->mItemType)
	{
		for (int i = 0; i < nOutSize; ++ i)
		{
			sendInsertItemObjNotify(nOutItemIndex[i]);
		}
	}
	else
	{
		sendInsertItemNotify(nItemID, nOutItemIndex, nOutItemNum, nOutSize);
	}


	return SUCCESS;
}

///  拥有道具的数目（注意，包括货币道具）
int CItemUnit::hasItem(int nItemID)
{
	if (nItemID <= MAX_CURRENCY_ID)
	{
		return getCurrency(CURRENCY_ID_2_TYPE(nItemID));
	}
	else
	{
		return mBag.hasItem(nItemID);
	}
}

/// 删除道具
int CItemUnit::removeItem(int nIndex, int nItemNum)
{
	int nResult = mBag.removeItem(nIndex, nItemNum);
	if (SUCCESS != nResult)
	{
		return nResult;
	}
	CItemObject* pItemObject = mBag.getItem(nIndex);

	CRemoveItemNotify tRemoveItemNotify;
	if (NULL == pItemObject)
	{
		tRemoveItemNotify.set_index(nIndex);
		tRemoveItemNotify.set_number(0);
	}
	else
	{
		tRemoveItemNotify.set_index(nIndex);
		tRemoveItemNotify.set_number(pItemObject->GetItemNum());
	}

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_REMOVE_ITEM, &tRemoveItemNotify);

	return SUCCESS;
}

/// 删除道具（注意，包括货币道具）
void CItemUnit::removeItemByID(int nItemID, int nItemNum)
{
	if (nItemID <= MAX_CURRENCY_ID)
	{
		consumeCurrency((EmCurrencyType)CURRENCY_ID_2_TYPE(nItemID), nItemNum);
	}
	else
	{
		removeItemOnly(nItemID, nItemNum);
	}
}

/// 删除道具（注意，不处理货币道具）,首先保证有这么多道具,这个接口应该用得不多（如果很多需要优化一下消息）
void CItemUnit::removeItemOnly(int nItemID, int nItemNum)
{
	int nOutItemIndex[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutItemNum[MAX_CONTAINER_ITEM_NUM] = { 0 };
	int nOutSize = 0;

	mBag.removeItem(nItemID, nItemNum, nOutItemIndex, nOutItemNum, nOutSize);
	for (int i = 0; i < nOutSize; ++ i)
	{
		CItemObject* pItemObject = mBag.getItem(nOutItemIndex[i]);
		CRemoveItemNotify tRemoveItemNotify;

		if (NULL == pItemObject)
		{
			tRemoveItemNotify.set_index(nOutItemIndex[i]);
			tRemoveItemNotify.set_number(0);
		}
		else
		{
			tRemoveItemNotify.set_index(nOutItemIndex[i]);
			tRemoveItemNotify.set_number(pItemObject->GetItemNum());
		}
		//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_REMOVE_ITEM, &tRemoveItemNotify);
	}
}

/// 道具是否适合玩家
bool CItemUnit::checkMatchItem(int nItemID, int nMetier)
{
	return true;
}


// 发送插入道具通知
void CItemUnit::sendInsertItemNotify(int nItemID, int* pIndex, int* pNumber, int nSize)
{
	CInsertItemNotify tInsertItemNotify;
	tInsertItemNotify.set_itemid(nItemID);
	for (int i = 0; i < nSize; ++ i)
	{
		tInsertItemNotify.add_index(pIndex[i]);
		tInsertItemNotify.add_number(pNumber[i]);
	}

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_INSERT_ITEM, &tInsertItemNotify);
}

///  插入道具通知(有特殊属性的道具)
void CItemUnit::sendInsertItemObjNotify(int nIndex)
{
	CItemObject* pItemObject = mBag.getItem(nIndex);
	if (NULL == pItemObject)
	{
		return;
	}

	CInsertItemObjNotify tInsertItemObjNotify;
	PBItemObject* pbItemObject = tInsertItemObjNotify.mutable_itemobject();
	if (NULL != pbItemObject)
	{
		pbItemObject->set_index(nIndex);
		pItemObject->createToPB(pbItemObject);
	}
	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_INSERT_ITEM_OBJ, &tInsertItemObjNotify);
}

/// 使用道具
void CItemUnit::onUseItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CUseItemRequest* pUseItemRequest = static_cast<CUseItemRequest*>(pMessage);
	MYTH_ASSERT(NULL != pUseItemRequest, return);
	
	int nItemIndex = pUseItemRequest->index();
	int nItemNum = pUseItemRequest->number();
	if (nItemNum <= 0)
	{
		sendUseItemResponse(ERR_PARAMETER_INVALID);
		return;
	}

	CItemObject* pItemObject = mBag.getItem(nItemIndex);
	if (NULL == pItemObject)
	{
		sendUseItemResponse(ERR_ITEM_INDEX_OBJ_DATA_NULL);
		return;
	}

	CTplItem* pTplITem = static_cast<CTplItem*>(CStaticData::searchTpl(pItemObject->GetItemID()));
	if (NULL == pTplITem)
	{
		sendUseItemResponse(ERR_TEMPLATE_INVALID);
		return;
	}

	if (emTemplateType_Item != pTplITem->mTemplateType)
	{
		sendUseItemResponse(ERR_ITEM_TEMPLATE_IS_NOT_ITEM);
		return;
	}

	if (pItemObject->GetItemNum() < nItemNum)
	{
		sendUseItemResponse(ERR_ITEM_INDEX_NUM_NOT_ENOUGH);
		return;
	}

	int nResult = SUCCESS;
	//switch(pTplITem->mItemType)
	//{
	//	
	//}

	sendUseItemResponse(nResult);
}

void CItemUnit::sendUseItemResponse(int nResult)
{
	CUseItemResponse tUseItemResponse;
	tUseItemResponse.set_result(nResult);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_USE_ITEM, &tUseItemResponse);
}

/// 卖出道具
void CItemUnit::onSellItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CSellItemRequest* pSellItemRequest = static_cast<CSellItemRequest*>(pMessage);
	MYTH_ASSERT(NULL != pSellItemRequest, return);

	int nItemIndex = pSellItemRequest->index();


	CItemObject* pItemObject = mBag.getItem(nItemIndex);
	if (NULL == pItemObject)
	{
		sendSellItemResponse(ERR_ITEM_INDEX_OBJ_DATA_NULL);
		return;
	}

	CTplItem* pTplITem = static_cast<CTplItem*>(CStaticData::searchTpl(pItemObject->GetItemID()));
	if (NULL == pTplITem)
	{
		sendSellItemResponse(ERR_TEMPLATE_INVALID);
		return;
	}

	if (emTemplateType_Item != pTplITem->mTemplateType)
	{
		sendSellItemResponse(ERR_ITEM_TEMPLATE_IS_NOT_ITEM);
		return;
	}

	int nMoney = pTplITem->mSellPrice * pItemObject->GetItemNum();
	obtainCurrency(emCurrency_Money, nMoney);
	removeItem(nItemIndex, pItemObject->GetItemNum());
	sendSellItemResponse(SUCCESS);
}

void CItemUnit::sendSellItemResponse(int nResult)
{
	CSellItemResponse tSellItemResponse;
	tSellItemResponse.set_result(nResult);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_SELL_ITEM, &tSellItemResponse);
}

/// 获得商店道具信息
void CItemUnit::onGetShopInfoRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CGetShopInfoRequest* pRequest = static_cast<CGetShopInfoRequest*>(pMessage);
	MYTH_ASSERT(NULL != pRequest, return);

	int nShopType = pRequest->shoptype();
	if (nShopType < emShopType_Common || nShopType >= emShopTypeMax)
	{
		return;
	}
	SHOP_CONFIG_LIST& rShopList = CItemModule::Inst()->getShopList();

	CShopLevelData* pLevelData = rShopList[nShopType].GetLevelData(mPlayer.getLevel());
	sendGetShopInfoResponse(SUCCESS, pLevelData);
}

void CItemUnit::sendGetShopInfoResponse(int nResult, CShopLevelData* pLevelData)
{
	CGetShopInfoResponse tResponse;
	tResponse.set_result(nResult);
	if (NULL != pLevelData)
	{
		for (uint i = 0; i < pLevelData->mGoodsList.size(); ++i)
		{
			PBShopGoods* pGoods = tResponse.add_goods();
			if (NULL == pGoods)
			{
				continue;
			}
			pLevelData->mGoodsList[i].createToPB(pGoods);
		}
	}

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_GET_SHOP_INFO, &tResponse);
}

/// 购买道具
void CItemUnit::onPurchaseItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CPurchaseItemRequest* pPurchaseItemRequest = static_cast<CPurchaseItemRequest*>(pMessage);
	EmShopType eShopType = (EmShopType)pPurchaseItemRequest->shoptype();
	uint nIndex = pPurchaseItemRequest->index();
	int nNum = pPurchaseItemRequest->num();

	if (eShopType < emShopType_Common || eShopType >= emShopTypeMax)
	{
		return;
	}

	SHOP_CONFIG_LIST& rShopList = CItemModule::Inst()->getShopList();
	CShopLevelData* pLevelData = rShopList[eShopType].GetLevelData(mPlayer.getLevel());
	if (NULL == pLevelData)
	{
		return;
	}

	if (nIndex < 0 || nIndex >= pLevelData->mGoodsList.size())
	{
		return;
	}

	CShopGoods& rGoods = pLevelData->mGoodsList[nIndex];
	// 货币不足
	if (hasItem(rGoods.mConsumeID) < rGoods.mConsumeNum)
	{
		return;
	}

	// 背包空间不足
	if (!checkItemSpace(&rGoods.mGoodsID, &rGoods.mGoodsNum, 1))
	{
		return;
	}

	removeItemByID(rGoods.mConsumeID, rGoods.mConsumeNum);
	insertItem(rGoods.mGoodsID, rGoods.mGoodsNum);
	sendPurchaseItemResponse(SUCCESS);
}

void CItemUnit::sendPurchaseItemResponse(int nResult)
{
	CPurchaseItemResponse tPurchaseItemResponse;
	tPurchaseItemResponse.set_result(nResult);
	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_PURCHASE_ITEM, &tPurchaseItemResponse);

}

/// 装备道具
void CItemUnit::onEquipItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CEquipItemRequest* pEquipItemRequest = static_cast<CEquipItemRequest*>(pMessage);

	int nItemIndex = pEquipItemRequest->itemindex();
	int nEquipPart = 0;
	int nResult = mEquip.equip(mPlayer, mBag, nItemIndex, nEquipPart);
	if (SUCCESS != nResult)
	{
		sendEquipItemResponse(nResult, nItemIndex);
		return;
	}
	broadcastChangeNotify(mPlayer.getObjID(), nEquipPart, mEquip.getItemID(nEquipPart));
	sendEquipItemResponse(SUCCESS, nItemIndex);
	mPlayer.refreshBaseProperty();
}

void CItemUnit::sendEquipItemResponse(int nResult, int nItemIndex)
{
	CEquipItemResponse tEquipItemResponse;
	tEquipItemResponse.set_result(nResult);
	tEquipItemResponse.set_itemindex(nItemIndex);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_EQUIP_ITEM, &tEquipItemResponse);
}

/// 卸载道具
void CItemUnit::onUnEquipItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CUnEquipItemRequest* pUnEquipItemRequest = static_cast<CUnEquipItemRequest*>(pMessage);
	int nEquipPart = pUnEquipItemRequest->equippart();
	int nItemIndex = pUnEquipItemRequest->itemindex();
	int nResult = mEquip.unequip(mPlayer, nEquipPart, mBag, nItemIndex);
	if (SUCCESS != nResult)
	{
		sendUnEquipItemResponse(nResult, nEquipPart, nItemIndex);
		return;
	}
	sendUnEquipItemResponse(SUCCESS, nEquipPart, nItemIndex);
	//broadcastChangeNotify(mPlayer.getObjID(), nEquipPart, mEquip.getItemID(nEquipPart));
	mPlayer.refreshBaseProperty();
}

/// 卸载装备回应
void CItemUnit::sendUnEquipItemResponse(int nResult, int nEquipPart, int nItemIndex)
{
	CUnEquipItemResponse tUnEquipItemResponse;
	tUnEquipItemResponse.set_result(nResult);
	tUnEquipItemResponse.set_equippart(nEquipPart);
	tUnEquipItemResponse.set_itemindex(nItemIndex);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_UNEQUIP_ITEM, &tUnEquipItemResponse);
}

/// 广播装备改变通知
void CItemUnit::broadcastChangeNotify(int nEntityID, int nEquipPart, int nEquipItemID)
{
	CEquipChangeNotify tEquipChangeNotify;
	tEquipChangeNotify.set_entityid(nEntityID);
	tEquipChangeNotify.set_equippart(nEquipPart);
	tEquipChangeNotify.set_equipitemid(nEquipItemID);

	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_NOTIYF_EQUIP_CHANGE, &tEquipChangeNotify);
}

// 拾取道具的请求
void CItemUnit::onPickItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CPickItemRequest* pPickItemRequest = static_cast<CPickItemRequest*>(pMessage);

	int nEntityID = pPickItemRequest->entityid();
	CEntity* pEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(nEntityID));
	if (NULL == pEntity)
	{
		return;
	}

	if (!pEntity->isItem())
	{
		return;
	}

	CTemplate* pTemplate = static_cast<CTemplate*>(CStaticData::searchTpl(pEntity->getTempID()));
	if (NULL == pTemplate)
	{
		return;
	}

	if (pTemplate->mTemplateType != emTemplateType_Item)
	{
		return;
	}
	CEntityItem* pEntityItem = static_cast<CEntityItem*>(pEntity);
	int nItemID = pEntityItem->getTempID();
	int nItemNum = pEntityItem->getItemNum();

	bool bResult =  checkItemSpace(&nItemID, &nItemNum, 1);
	if (!bResult)
	{
		sendPickItemResponse(ERR_BAGGAGE_IS_FULL);
		return ;
	}

	insertAllItem(&nItemID, &nItemNum, 1);
	sendPickItemResponse(SUCCESS);
}

// 拾取道具的回应
void CItemUnit::sendPickItemResponse(int nResult)
{
	CPickItemResponse tPickItemResponse;
	tPickItemResponse.set_result(nResult);
	//CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_PICK_ITEM, &tPickItemResponse);
}