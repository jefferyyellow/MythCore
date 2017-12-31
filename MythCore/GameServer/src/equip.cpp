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

	CTplItem* pTplItem = reinterpret_cast<CTplItem*>(CStaticData::searchTpl(pItemObject->GetItemID()));
	if (NULL == pTplItem)
	{
		return ERR_TEMPLATE_INVALID;
	}

	int nEquipPart = -1;
	CItemObject* pDstItemObject = NULL;
	switch (pTplItem->mItemType)
	{
		case emItemType_Equip:
		{
			CTplEquip* pTplEquip = reinterpret_cast<CTplEquip*>(pTplItem);
			if (pTplEquip->mEquipPart < 0 && pTplEquip->mEquipPart >= MAX_EQUIP_LIST)
			{
				return ERR_EQUIP_PART_IS_INVALID;
			}

			if (rPlayer.getLevel() < pTplEquip->mLevelRequire)
			{
				return ERR_EQUIP_PLAYER_LEVEL_NOT_ENOUGH;
			}

			int nDetObjID = getItemObjID(nEquipPart);
			pDstItemObject = reinterpret_cast<CItemObject*>(CObjPool::Inst()->getObj(nDetObjID));
			nEquipPart = pTplEquip->mEquipPart;
			rEquipPart = nEquipPart;
			break;
		}
		default:
		{
			return ERR_EQUIP_ITEM_CANNOT_EQUIP;
			break;
		}
	}

	if (NULL != pDstItemObject)
	{
		rBox.setItem(nBoxIndex, pDstItemObject->getObjID(), pDstItemObject->GetItemID());
	}
	else
	{
		rBox.setItem(nBoxIndex, INVALID_OBJ_ID, 0);
	}
	setItem(nEquipPart, pItemObject->getObjID(), pItemObject->GetItemID());
	
	return SUCCESS;
}

int CEquipList::unequip(CEntityPlayer& rPlayer, int nEquipPart, CItemBox& rBox, int nBoxIndex)
{
	if (nEquipPart < 0 || nEquipPart >= MAX_EQUIP_LIST)
	{
		return ERR_EQUIP_PART_IS_INVALID;
	}
	CItemObject* pDstObject = rBox.getItem(nBoxIndex);
	if (NULL != pDstObject)
	{
		return ERR_EQUIP_DES_INDEX_DATA_NOT_NULL;
	}

	// 装备上没有这个道具
	int nDetObjID = getItemObjID(nEquipPart);
	if (INVALID_OBJ_ID == nDetObjID)
	{
		return ERR_EQUIP_SRC_ITEM_DATA_NULL;
	}

	// 道具为空
	CItemObject* pItemObject = reinterpret_cast<CItemObject*>(CObjPool::Inst()->getObj(nDetObjID));
	if (NULL == pItemObject)
	{
		return ERR_EQUIP_SRC_ITEM_DATA_NULL;
	}

	setItem(nEquipPart, INVALID_OBJ_ID, 0);
	rBox.setItem(nBoxIndex, pItemObject->getObjID(), pItemObject->GetItemID());
	return SUCCESS;
}

int CEquipList::getProperty(int nPropertyType)
{
	int nPropertyValue = 0;
	for (int i = 0; i < MAX_EQUIP_LIST; ++ i)
	{
		CItemEquip* pEquip = reinterpret_cast<CItemEquip*>(getItem(i));
		if (NULL == pEquip)
		{
			continue;
		}
		nPropertyValue += pEquip->getProperty(nPropertyType);
	}

	return nPropertyValue;
}