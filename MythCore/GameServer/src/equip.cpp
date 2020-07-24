#include "equip.h"
#include "entityplayer.h"
#include "itemcontainer.h"
#include "itemobject.h"
#include "errcode.h"
int CEquipList::equip(CEntityPlayer& rPlayer, CItemBox& rBox, int nBoxIndex, int& rEquipPart)
{
	CItemObject* pItemObject = rBox.getItem(nBoxIndex);
	if (NULL == pItemObject)
	{
		return ERR_EQUIP_SRC_ITEM_DATA_NULL;
	}

	CTplItem* pTplItem = static_cast<CTplItem*>(CStaticData::searchTpl(pItemObject->GetItemID()));
	if (NULL == pTplItem)
	{
		return ERR_TEMPLATE_INVALID;
	}

	int nEquipPart = -1;
	CItemObject* pDestItemObject = NULL;
	switch (pTplItem->mItemType)
	{
		case emItemType_Equip:
		{
			CTplEquip* pTplEquip = static_cast<CTplEquip*>(pTplItem);
			nEquipPart = pTplEquip->mEquipPart;
			if (nEquipPart < 0 || nEquipPart >= emEquipPartMax)
			{
				return ERR_EQUIP_PART_IS_INVALID;
			}

			if (rPlayer.getLevel() < pTplEquip->mLevelRequire)
			{
				return ERR_EQUIP_PLAYER_LEVEL_NOT_ENOUGH;
			}

			int nDetObjID = getItemObjID(nEquipPart);
			pDestItemObject = static_cast<CItemObject*>(CObjPool::Inst()->getObj(nDetObjID));
			rEquipPart = nEquipPart;
			break;
		}
		default:
		{
			return ERR_EQUIP_ITEM_CANNOT_EQUIP;
			break;
		}
	}

	if (NULL != pDestItemObject)
	{
		if (emItemType_Equip == pTplItem->mItemType)
		{
			CItemEquip* pDestItemEquip = static_cast<CItemEquip*>(pDestItemObject);
			pDestItemEquip->setPropertyDirty(rPlayer);
		}
		rBox.setItem(nBoxIndex, pDestItemObject->getObjID(), pDestItemObject->GetItemID());
	}
	else
	{
		rBox.setItem(nBoxIndex, INVALID_OBJ_ID, 0);
	}
	setItem(nEquipPart, pItemObject->getObjID(), pItemObject->GetItemID());
	if (emItemType_Equip == pTplItem->mItemType)
	{
		CItemEquip* pItemEquip = static_cast<CItemEquip*>(pItemObject);
		pItemEquip->refreshProperty();
		pItemEquip->setPropertyDirty(rPlayer);
	}
	return SUCCESS;
}

int CEquipList::unequip(CEntityPlayer& rPlayer, int nEquipPart, CItemBox& rBox, int nBoxIndex)
{
	if (nEquipPart < 0 || nEquipPart >= emEquipPartMax)
	{
		return ERR_EQUIP_PART_IS_INVALID;
	}
	CItemObject* pDestObject = rBox.getItem(nBoxIndex);
	if (NULL != pDestObject)
	{
		return ERR_EQUIP_DES_INDEX_DATA_NOT_NULL;
	}

	// װ����û���������
	int nDetObjID = getItemObjID(nEquipPart);
	if (INVALID_OBJ_ID == nDetObjID)
	{
		return ERR_EQUIP_SRC_ITEM_DATA_NULL;
	}

	// ����Ϊ��
	CItemObject* pItemObject = static_cast<CItemObject*>(CObjPool::Inst()->getObj(nDetObjID));
	if (NULL == pItemObject)
	{
		return ERR_EQUIP_SRC_ITEM_DATA_NULL;
	}

	CTplItem* pTplItem = static_cast<CTplItem*>(CStaticData::searchTpl(pItemObject->GetItemID()));
	if (NULL == pTplItem)
	{
		return ERR_TEMPLATE_INVALID;
	}

	setItem(nEquipPart, INVALID_OBJ_ID, 0);
	rBox.setItem(nBoxIndex, pItemObject->getObjID(), pItemObject->GetItemID());

	if (emItemType_Equip == pTplItem->mItemType)
	{
		static_cast<CItemEquip*>(pItemObject)->setPropertyDirty(rPlayer);
	}
	return SUCCESS;
}

int CEquipList::getProperty(int nPropertyType)
{
	int nPropertyValue = 0;
	for (int i = 0; i < MAX_EQUIP_LIST; ++ i)
	{
		CItemEquip* pEquip = static_cast<CItemEquip*>(getItem(i));
		if (NULL == pEquip)
		{
			continue;
		}
		nPropertyValue += pEquip->getProperty(nPropertyType);
	}

	return nPropertyValue;
}

void CEquipList::refreshProperty()
{
	int nPropertyValue = 0;
	for (int i = 0; i < MAX_EQUIP_LIST; ++i)
	{
		CItemEquip* pEquip = static_cast<CItemEquip*>(getItem(i));
		if (NULL == pEquip)
		{
			continue;
		}
		pEquip->refreshProperty();
	}

}
