#include "itemunit.h"
#include "itemmodule.hxx.pb.h"
#include "scenejob.h"
#include "errcode.h"
#include "itemobject.h"
#include "template.h"
#include "locallogjob.h"

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

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_CURRENCY_UPDATE, &tMsg);
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
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_CURRENCY_UPDATE, &tMsg);
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
int CItemUnit::insertItem(int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pItemID || NULL == pItemNum)
	{
		return ERR_PARAMETER_INVALID;
	}
	
	for (int i = 0; i < nSize; ++ i)
	{
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
	
	CTplItem* pTplITem = (CTplItem*)CStaticData::searchTpl(nItemID);
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
		sendInsertItemObjNotify(nOutItemIndex[0]);
	}
	else
	{
		sendInsertItemNotify(nItemID, nOutItemIndex, nOutItemNum, nOutSize);
	}


	return SUCCESS;
}

/// 拥有道具的数目
int CItemUnit::hasItem(int nItemID)
{
	return mBag.hasItem(nItemID);
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

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_REMOVE_ITEM, &tRemoveItemNotify);

	return SUCCESS;
}

/// 删除道具,首先保证有这么多道具,这个接口应该用得不多（如果很多需要优化一下消息）
void CItemUnit::removeItemByID(int nItemID, int nItemNum)
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
		CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_REMOVE_ITEM, &tRemoveItemNotify);
	}
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

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_INSERT_ITEM, &tInsertItemNotify);
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
	pItemObject->createToPB(tInsertItemObjNotify.mutable_itemobject());
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_INSERT_ITEM_OBJ, &tInsertItemObjNotify);
}

/// 使用道具
void CItemUnit::onUseItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CUseItemRequest* pUseItemRequest = reinterpret_cast<CUseItemRequest*>(pMessage);
	MYTH_ASSERT(NULL == pUseItemRequest, return);
	
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

	CTplItem* pTplITem = (CTplItem*)CStaticData::searchTpl(pItemObject->GetItemID());
	if (NULL == pTplITem)
	{
		sendUseItemResponse(ERR_TEMPLATE_INVALID);
		return;
	}

	if (emTemplateType_Item != pTplITem->mTemplateType)
	{
		sendUseItemResponse(ERR_TEMPLATE_IS_NOT_ITEM);
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

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_USE_ITEM, &tUseItemResponse);
}

/// 卖出道具
void CItemUnit::onSellItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CSellItemRequest* pSellItemRequest = reinterpret_cast<CSellItemRequest*>(pMessage);
	MYTH_ASSERT(NULL == pSellItemRequest, return);

	int nItemIndex = pSellItemRequest->index();


	CItemObject* pItemObject = mBag.getItem(nItemIndex);
	if (NULL == pItemObject)
	{
		sendSellItemResponse(ERR_ITEM_INDEX_OBJ_DATA_NULL);
		return;
	}

	CTplItem* pTplITem = (CTplItem*)CStaticData::searchTpl(pItemObject->GetItemID());
	if (NULL == pTplITem)
	{
		sendSellItemResponse(ERR_TEMPLATE_INVALID);
		return;
	}

	if (emTemplateType_Item != pTplITem->mTemplateType)
	{
		sendSellItemResponse(ERR_TEMPLATE_IS_NOT_ITEM);
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

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_SELL_ITEM, &tSellItemResponse);
}

/// 购买道具
void CItemUnit::onPurchaseItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CPurchaseItemRequest* pPurchaseItemRequest = reinterpret_cast<CPurchaseItemRequest*>(pMessage);
	MYTH_ASSERT(NULL == pPurchaseItemRequest, return);

	int nIndex = pPurchaseItemRequest->index();
	int nNum = pPurchaseItemRequest->num();

	// 建议商店还是走xml方式


	sendPurchaseItemResponse(SUCCESS);
}

void CItemUnit::sendPurchaseItemResponse(int nResult)
{
	CPurchaseItemResponse tPurchaseItemResponse;
	tPurchaseItemResponse.set_result(nResult);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_PURCHASE_ITEM, &tPurchaseItemResponse);

}

/// 装备道具
void CItemUnit::onEquipItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CEquipItemRequest* pEquipItemRequest = reinterpret_cast<CEquipItemRequest*>(pMessage);
	MYTH_ASSERT(NULL == pEquipItemRequest, return);

	int nItemIndex = pEquipItemRequest->itemindex();


	sendEquipItemResponse(SUCCESS, nItemIndex);
}

void CItemUnit::sendEquipItemResponse(int nResult, int nItemIndex)
{
	CEquipItemResponse tEquipItemResponse;
	tEquipItemResponse.set_result(nResult);
	tEquipItemResponse.set_itemindex(nItemIndex);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_EQUIP_ITEM, &tEquipItemResponse);
}

/// 卸载道具
void CItemUnit::onUnEquipItemRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL == pMessage, return);
	CUnEquipItemRequest* pUnEquipItemRequest = reinterpret_cast<CUnEquipItemRequest*>(pMessage);
	MYTH_ASSERT(NULL == pUnEquipItemRequest, return);

}

void CItemUnit::sendUnEquipItemResponse(int nResult, int nEquipIndex, int nItemIndex)
{
	CUnEquipItemResponse tUnEquipItemResponse;
	tUnEquipItemResponse.set_result(nResult);
	tUnEquipItemResponse.set_equipindex(nEquipIndex);
	tUnEquipItemResponse.set_itemindex(nItemIndex);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_UNEQUIP_ITEM, &tUnEquipItemResponse);
}
