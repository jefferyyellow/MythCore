#include "itemunit.h"
#include "itemmodule.hxx.pb.h"
#include "scenejob.h"
#include "errcode.h"
#include "itemobject.h"
#include "template.h"
#include "locallogjob.h"
/// ��ý�Ǯ
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

/// ���ѽ�Ǯ
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

/// �����ʯ
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

/// ������ʯ 
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

/// �����Ƿ���װ����Щ����
bool CItemUnit::checkItemSpace(int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pItemID || NULL == pItemNum)
	{
		return false;
	}

	return mBag.checkSpace(pItemID, pItemNum, nSize);
}

/// �������
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

/// �������
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

	// ͨ�õĵ���û����������
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

/// ӵ�е��ߵ���Ŀ
int CItemUnit::hasItem(int nItemID)
{
	return mBag.hasItem(nItemID);
}

/// ɾ������
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

/// ɾ������,���ȱ�֤����ô�����,����ӿ�Ӧ���õò��ࣨ����ܶ���Ҫ�Ż�һ����Ϣ��
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

// ���Ͳ������֪ͨ
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

///  �������֪ͨ(���������Եĵ���)
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

/// ʹ�õ���
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

/// ��������
void CItemUnit::onSellItemRequest(Message* pMessage)
{
	
}

void CItemUnit::sendSellItemResponse(int nResult)
{
	CSellItemResponse tSellItemResponse;
	tSellItemResponse.set_result(nResult);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_SELL_ITEM, &tSellItemResponse);
}

/// �������
void CItemUnit::onPurchaseItemRequest(Message* pMessage)
{
	
}

void CItemUnit::sendPurchaseItemResponse(int nResult)
{
	CPurchaseItemResponse tPurchaseItemResponse;
	tPurchaseItemResponse.set_result(nResult);
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_PURCHASE_ITEM, &tPurchaseItemResponse);

}