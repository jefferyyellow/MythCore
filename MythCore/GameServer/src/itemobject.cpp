#include "itemobject.h"
#include "template.h"
CItemObject* CItemFactory::createItem(int nItemID)
{
	CTplItem* pTplItem = (CTplItem*)CStaticData::SearchTpl(nItemID);
	if (NULL == pTplItem)
	{
		return NULL;
	}
	CItemObject* pObject = NULL;
	return pObject;
}