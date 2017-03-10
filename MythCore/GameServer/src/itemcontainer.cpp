#include "itemcontainer.h"
#include "template.h"
#include "itemobject.h"
#include "objpool.h"
bool CItemContainer::checkSpace(int* pItemID, int* pNumber, int nSize)
{
	if (nSize > MAX_INSERT_TYPE_NUM)
	{
		return;
	}

	int nNum[MAX_INSERT_TYPE_NUM] = {0};
	int nPileLimit[MAX_INSERT_TYPE_NUM] = {0};
	for (int i = 0; i < nSize; ++i)
	{
		nNum[i] = pNumber[i];
		CTplItem* pTplItem = (CTplItem*)CDataStatic::SearchTpl(pItemID[i]);
		if (NULL == pTplItem)
		{
			nPileLimit[i] = 0;
			continue;
		}
		nPileLimit[i] = pTplItem->mPileLimit;
	}

	int nAddSpace = 0;
	int nTotalSpace = 0;
	int nTmpSize = nSize;
	for (int i = 0; i < mSize; ++ i)
	{
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nAddSpace = 1;
			for (int j = 0; j < nSize; ++j)
			{
				if (nPileLimit[j] == 1 && nNum[i] > 0)
				{
					nAddSpace = 0;
					-- nNum[i];
					if (nNum[i] <= 0)
					{
						-- nTmpSize;
					}
				}
			}
		}
		else
		{
			for (int j = 0; j < nSize; ++j)
			{
				if (pItemID[i] == mItemID[j] && nNum[i] > 0)
				{
					CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
					if (NULL != pItemObject)
					{
						nNum[i] -= nPileLimit[j] - pItemObject->GetItemNum();
						if (nNum[i] < 0)
						{
							 -- nTmpSize;
						}
					}
				}
			}
		}

		if (nTmpSize < 0)
		{
			break;
		}
		nTotalSpace += nAddSpace;
	}

	int nNeedSpace = 0;
	for (int j = 0; j < nSize; ++j)
	{
		
	}
}

bool CItemContainer::checkSpace(int nItemID, int nNumber)
{
	CTplItem* pTplItem = (CTplItem*)CDataStatic::SearchTpl(nItemID);
	if (NULL == pTplItem)
	{
		return false;
	}

	for (int i = 0; i < mSize; ++ i)
	{
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nNumber -= pTplItem->mPileLimit;
		}
		else if (nItemID == mItemID[i])
		{
			CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
			if (NULL != pItemObject)
			{
				nNumber -= pTplItem->mPileLimit - pItemObject->GetItemNum();
			}
		}
		if (nNumber <= 0)
		{
			return true;
		}
	}

	return false;
}
