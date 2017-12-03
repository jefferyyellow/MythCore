#include "itemobject.h"
#include "template.h"
#include "objpool.h"
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
	
}

void CItemCommon::createToPB(PBItemObject* pItemObject)
{
	
}


void CItemEquip::setFromPB(PBItemObject* pItemObject)
{
	
}

void CItemEquip::createToPB(PBItemObject* pItemObject)
{

}