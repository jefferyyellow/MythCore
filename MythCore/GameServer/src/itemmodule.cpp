#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
#include "itemmodule.hxx.pb.h"
#include "scenejob.h"

void CItemModule::OnTimer(unsigned int nTickOffset)
{

}

void CItemModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{

}

/// 获得金钱
int CItemModule::obtainMoney(CEntityPlayer* pPlayer, int nMoney)
{
	if (nMoney < 0)
	{
		return ERROR_ITEM_OBTAIN_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	CItemUnit& rItemUnit = pPlayer->GetItemUnit();
	int nCurrMoney = rItemUnit.getMoney() + nMoney;
	rItemUnit.setMoney(nCurrMoney > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrMoney);

	CMessagePlayerMoneyUpdateNotify tMsg;
	tMsg.set_money(rItemUnit.getMoney());
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费金钱
int CItemModule::consumeMoney(CEntityPlayer* pPlayer, int nMoney)
{
	if (nMoney < 0)
	{
		return ERROR_ITEM_CONSUME_MONEY_INVALID;
	}
	if (0 == nMoney)
	{
		return SUCCESS;
	}

	CItemUnit& rItemUnit = pPlayer->GetItemUnit();
	if (rItemUnit.getMoney() < nMoney)
	{
		return ERROR_MONEY_NOT_ENOUGH;
	}

	rItemUnit.setMoney(rItemUnit.getMoney() - nMoney);

	CMessagePlayerMoneyUpdateNotify tMsg;
	tMsg.set_money(rItemUnit.getMoney());
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE, &tMsg);
	return SUCCESS;
}

/// 获得钻石
int	CItemModule::obtainDiamond(CEntityPlayer* pPlayer, int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERROR_ITEM_OBTAIN_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}

	CItemUnit& rItemUnit = pPlayer->GetItemUnit();
	int nCurrDiamond = rItemUnit.getDiamond() + nDiamond;
	rItemUnit.setDiamond(nCurrDiamond > PLAYER_MONEY_LIMIT ? PLAYER_MONEY_LIMIT : nCurrDiamond);

	CMessagePlayerDiamondUpdateNotify tMsg;
	tMsg.set_diamond(rItemUnit.getDiamond());
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE, &tMsg);
	return SUCCESS;
}

/// 消费钻石 
int	CItemModule::consumeDiamond(CEntityPlayer* pPlayer, int nDiamond)
{
	if (nDiamond < 0)
	{
		return ERROR_ITEM_CONSUME_DIAMOND_INVALID;
	}
	if (0 == nDiamond)
	{
		return SUCCESS;
	}
	CItemUnit& rItemUnit = pPlayer->GetItemUnit();
	if (rItemUnit.getDiamond() < nDiamond)
	{
		return ERROR_DIAMOND_NOT_ENOUGH;
	}

	rItemUnit.setDiamond(rItemUnit.getDiamond() - nDiamond);

	CMessagePlayerDiamondUpdateNotify tMsg;
	tMsg.set_diamond(rItemUnit.getDiamond());
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE, &tMsg);
	return SUCCESS;
}

/// 包裹是否能装下这些道具
bool CItemModule::checkItemSpace(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum, int nSize)
{
	if (NULL == pPlayer || NULL == pItemID || NULL == pItemNum)
	{
		return false;
	}

	CItemBox& rBox = pPlayer->GetItemUnit().getBag();
	return rBox.checkSpace(pItemID, pItemNum, nSize);
}

/// 插入道具
int CItemModule::insertItem(CEntityPlayer* pPlayer, int* pItemID, int* pItemNum, int nSize)
{

	return SUCCESS;
}
