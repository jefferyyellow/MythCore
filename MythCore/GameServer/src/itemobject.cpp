#include "itemobject.h"
#include "template.h"
#include "objpool.h"
#include "itemmodule.hxx.pb.h"
#include "entityplayer.h"
CItemObject* CItemFactory::createItem(int nItemID)
{
	CTplItem* pTplItem = static_cast<CTplItem*>(CStaticData::searchTpl(nItemID));
	if (NULL == pTplItem)
	{
		return NULL;
	}
	CItemObject* pObject = NULL;
	switch (pTplItem->mItemType)
	{
		//case emItemType_Common:
		//{
		//	pObject = (CItemObject*)CObjPool::Inst()->allocObj(emObjType_Item_Common);
		//	break;
		//}
		case emItemType_Equip:
		{
			pObject = (CItemObject*)CObjPool::Inst()->allocObj(emObjType_Item_Equip);
			break;
		}
	}
	return pObject;
}

void CItemFactory::destroyItem(int nObjID)
{
	CObjPool::Inst()->free(nObjID);
}

void CItemCommon::setFromPB(PBItemObject* pItemObject)
{
	if (NULL == pItemObject)
	{
		return;
	}
	SetItemID(pItemObject->itemid());
	SetItemNum(pItemObject->number());
}

void CItemCommon::createToPB(PBItemObject* pItemObject)
{
	if (NULL == pItemObject)
	{
		return;
	}
	pItemObject->set_itemid(GetItemID());
	pItemObject->set_number(GetItemNum());
}


void CItemEquip::setFromPB(PBItemObject* pItemObject)
{
	if (NULL == pItemObject)
	{
		return;
	}
	PBItemEquip* pbItemEqup = pItemObject->mutable_itemequip();
	if (NULL != pbItemEqup)
	{
		return;
	}

	SetItemID(pItemObject->itemid());
	SetItemNum(pItemObject->number());
	mLevel = pbItemEqup->level();

}

void CItemEquip::createToPB(PBItemObject* pItemObject)
{
	if (NULL == pItemObject)
	{
		return;
	}
	PBItemEquip* pbItemEqup = pItemObject->mutable_itemequip();
	if (NULL != pbItemEqup)
	{
		return;
	}
	pItemObject->set_itemid(GetItemID());
	pItemObject->set_number(GetItemNum());

	pbItemEqup->set_level(mLevel);
}

int	CItemEquip::getProperty(int nPropertyType)
{
	int nPropertyValue = 0;
	for (int i = 0; i < EQUIP_PROPERTY_NUM; ++ i)
	{
		if (emPropertyType_None == mPropertyType[i])
		{
			break;
		}

		if (nPropertyType == mPropertyType[i])
		{
			nPropertyValue = mPropertyValue[i];
		}
	}

	return nPropertyValue;
}

void CItemEquip::setPropertyDirty(CEntityPlayer& rPlayer)
{
	for (int i = 0; i < EQUIP_PROPERTY_NUM; ++ i)
	{
		if (emPropertyType_None == mPropertyType[i])
		{
			break;
		}
		rPlayer.setPropertyDirty((EmPropertyType)mPropertyType[i], true);
	}
}

// Ë¢ÐÂÊôÐÔ
void CItemEquip::refreshProperty()
{
	CTplEquip* pTplEquip = static_cast<CTplEquip*>(CStaticData::searchTpl(mItemID));
	if (NULL == pTplEquip)
	{
		return;
	}

	for (int i = 0; i < EQUIP_PROPERTY_NUM; ++ i)
	{
		if (emPropertyType_None == pTplEquip->mProperty[i].mType)
		{
			break;
		}
		mPropertyType[i] = pTplEquip->mProperty[i].mType;
		mPropertyValue[i] = pTplEquip->mProperty[i].mValue;
	}
}