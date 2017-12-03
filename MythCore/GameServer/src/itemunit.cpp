#include "itemunit.h"
#include "itemmodule.hxx.pb.h"
#include "scenejob.h"
#include "errcode.h"
#include "itemobject.h"
#include "template.h"
#include "locallogjob.h"
/// 获得金钱
int CItemUnit::obtainMoney(int nMoney)
{
	if (nMoney < 0)
	{
		return ERR_ITEM_OBTAIN_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	int nCurrMoney = mMoney + nMoney;
	mMoney = nCurrMoney > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrMoney;

	CMoneyUpdateNotify tMsg;
	tMsg.set_money(mMoney);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费金钱
int CItemUnit::consumeMoney(int nMoney)
{
	if (nMoney < 0)
	{
		return ERR_ITEM_CONSUME_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	if (mMoney < nMoney)
	{
		return ERR_MONEY_NOT_ENOUGH;
	}

	mMoney = mMoney - nMoney;

	CMoneyUpdateNotify tMsg;
	tMsg.set_money(mMoney);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 获得钻石
int	CItemUnit::obtainDiamond(int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERR_ITEM_OBTAIN_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}

	int nCurrDiamond = mDiamond + nDiamond;
	mDiamond = nCurrDiamond > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrDiamond;

	CDiamondUpdateNotify tMsg;
	tMsg.set_diamond(mDiamond);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_DIAMOND_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费钻石 
int	CItemUnit::consumeDiamond(int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERR_ITEM_CONSUME_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}
	if (mDiamond < nDiamond)
	{
		return ERR_DIAMOND_NOT_ENOUGH;
	}

	mDiamond = mDiamond - nDiamond;

	CDiamondUpdateNotify tMsg;
	tMsg.set_diamond(mDiamond);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_NOTIYF_DIAMOND_UPDATE, &tMsg);
	return SUCCESS;
}

/// 包裹是否能装下这些道具
bool CItemUnit::checkItemSpace(int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pItemID || NULL == pItemNum)
	{
		return false;
	}

	return mBag.checkSpace(pItemID, pItemNum, nSize);
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
		insertItem(pItemID[i], pItemNum[i]);
	}
	

	return SUCCESS;
}

/// 插入道具
int CItemUnit::insertItem(int nItemID, int nItemNum)
{
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
	mBag.removeItem(nIndex, nItemNum);
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
	
}

void CItemUnit::sendPurchaseItemResponse(int nResult)
{
	CPurchaseItemResponse tPurchaseItemResponse;
	tPurchaseItemResponse.set_result(nResult);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_PURCHASE_ITEM, &tPurchaseItemResponse);

}