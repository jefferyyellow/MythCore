#include "itemobject.h"
#include "template.h"
#include "objpool.h"
#include "itemmodule.hxx.pb.h"
CItemObject* CItemFactory::createItem(int nItemID)
{
	CTplItem* pTplItem = (CTplItem*)CStaticData::searchTpl(nItemID);
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
		if (nPropertyType == mPropertyType[i])
		{
			nPropertyValue = mPropertyValue[i];
		}
	}

	return nPropertyValue;
}