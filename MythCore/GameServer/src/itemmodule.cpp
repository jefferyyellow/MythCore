#include "itemmodule.hxx.pb.h"
#include "itemmodule.h"
#include "entityplayer.h"
#include "errcode.h"
#include "servercommon.h"

CItemModule::CItemModule()
{
	mShopList.reserve(emShopTypeMax);
}
CItemModule::~CItemModule()
{

}
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
void CItemModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
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
	}
}

int	CItemModule::getFromDropTable(int nDropTableID, int& rDropItemNum)
{
	// �õ�����ĵ����
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
	// �õ�����ID
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