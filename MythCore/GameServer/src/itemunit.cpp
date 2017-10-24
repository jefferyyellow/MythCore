#include "itemunit.h"
#include "itemmodule.hxx.pb.h"
#include "scenejob.h"
#include "errcode.h"

/// 获得金钱
int CItemUnit::obtainMoney(int nMoney)
{
	if (nMoney < 0)
	{
		return ERROR_ITEM_OBTAIN_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	int nCurrMoney = mMoney + nMoney;
	mMoney = nCurrMoney > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrMoney;

	CMessagePlayerMoneyUpdateNotify tMsg;
	tMsg.set_money(mMoney);
	CSceneJob::Inst()->sendClientMessage(mPlayer, ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费金钱
int CItemUnit::consumeMoney(int nMoney)
{
	if (nMoney < 0)
	{
		return ERROR_ITEM_CONSUME_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	if (mMoney < nMoney)
	{
		return ERROR_MONEY_NOT_ENOUGH;
	}

	mMoney = mMoney - nMoney;

	CMessagePlayerMoneyUpdateNotify tMsg;
	tMsg.set_money(mMoney);
	CSceneJob::Inst()->sendClientMessage(mPlayer, ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 获得钻石
int	CItemUnit::obtainDiamond(int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERROR_ITEM_OBTAIN_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}

	int nCurrDiamond = mDiamond + nDiamond;
	mDiamond = nCurrDiamond > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrDiamond;

	CMessagePlayerDiamondUpdateNotify tMsg;
	tMsg.set_diamond(mDiamond);
	CSceneJob::Inst()->sendClientMessage(mPlayer, ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费钻石 
int	CItemUnit::consumeDiamond(int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERROR_ITEM_CONSUME_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}
	if (mDiamond < nDiamond)
	{
		return ERROR_DIAMOND_NOT_ENOUGH;
	}

	mDiamond = mDiamond - nDiamond;

	CMessagePlayerDiamondUpdateNotify tMsg;
	tMsg.set_diamond(mDiamond);
	CSceneJob::Inst()->sendClientMessage(mPlayer, ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE, &tMsg);
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
		return ERROR_PARAMETER_INVALID;
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
	
	mBag.insertItem(nItemID, nItemNum, nOutItemIndex, nOutItemNum, nOutSize);
	return SUCCESS;
}
