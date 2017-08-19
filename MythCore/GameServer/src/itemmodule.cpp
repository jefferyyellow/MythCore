#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
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
	return SUCCESS;
}

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
	return SUCCESS;
}

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
	return SUCCESS;
}

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
	return SUCCESS;
}
