#include "itemcontainer.h"
#include "template.h"
#include "itemobject.h"
#include "objpool.h"
#include "locallogjob.h"
bool CItemContainer::checkSpace(int* pItemID, int* pNumber, int nSize)
{
	if (nSize > MAX_INSERT_TYPE_NUM)
	{
		return false;
	}

	if (nSize == 1)
	{
		return checkSpace(pItemID[0], pNumber[0]);
	}

	// 缓存道具数目和堆叠数目
	int nNum[MAX_INSERT_TYPE_NUM] = {0};
	int nPileLimit[MAX_INSERT_TYPE_NUM] = {0};
	for (int i = 0; i < nSize; ++i)
	{
		nNum[i] = pNumber[i];
		CTplItem* pTplItem = (CTplItem*)CStaticData::searchTpl(pItemID[i]);
		if (NULL == pTplItem)
		{
			nPileLimit[i] = 0;
			continue;
		}
		nPileLimit[i] = pTplItem->mPileLimit;
	}

	// 先将可以堆叠的道具放在对应的已经有的格子里面
	int nTotalSpace = 0;
	for (int i = 0; i < (int)mSize; ++ i)
	{
		// 空格子
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nTotalSpace += 1;
		}
		else
		{
			// 已经有道具的格子是否能放下这一堆东西里的任意一种还有的可以堆叠的道具
			// 如果可以，减少堆叠数量
			for (int j = 0; j < nSize; ++j)
			{
				if (mItemID[i] == pItemID[j] && nPileLimit[i] > 1 && nNum[j] > 0)
				{
					CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
					if (NULL != pItemObject)
					{
						nNum[i] -= nPileLimit[j] - pItemObject->GetItemNum();
					}
				}
			}
		}
	}

	// 计算所有堆叠数目为1的，和没有放下的堆叠数目大于1的道具
	// 计算所需要的空格子书面
	int nNeedSpace = 0;
	for (int i = 0; i < nSize; ++i)
	{
		if (nNum[i] > 0 && nPileLimit[i]  > 0)
		{
			if (nPileLimit[i] == 1)
			{
				nNeedSpace += nNum[i];
			}
			else
			{
				nNeedSpace += (nNum[i] + nPileLimit[i] - 1) / nPileLimit[i];
			}
		}
	}
	if (nNeedSpace > nTotalSpace)
	{
		return true;
	}

	return false;
}

// 如果是一种道具，方便很多，空格子可以直接用，不用考虑先堆叠
// 可以先使用空格子
bool CItemContainer::checkSpace(int nItemID, int nNumber)
{
	CTplItem* pTplItem = (CTplItem*)CStaticData::searchTpl(nItemID);
	if (NULL == pTplItem)
	{
		return true;
	}

	for (int i = 0; i < (int)mSize; ++ i)
	{
		// 直接用空格子
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nNumber -= pTplItem->mPileLimit;
		}
		// 堆叠
		else if (nItemID == mItemID[i])
		{
			CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
			if (NULL != pItemObject)
			{
				nNumber -= pTplItem->mPileLimit - pItemObject->GetItemNum();
			}
		}
		// 检测到可以放下了
		if (nNumber <= 0)
		{
			return true;
		}
	}

	return false;
}

int CItemContainer::insertItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen)
{
	if (NULL == pOutIndex || NULL == pOutNumber || 0 == nItemNum)
	{
		return -1;
	}

	// 如果数据模版找不到
	CTplItem* tpItem = (CTplItem*)CStaticData::searchTpl(nItemID);
	if (NULL == tpItem)
	{
		// 道具模板数据为空
		LOG_ERROR("item template data is null");
		return -2;
	}

	rOutLen = 0;
	int nPileLimit = tpItem->mPileLimit;
	
	int tEmpty[MAX_CONTAINER_ITEM_NUM] = { -1 };
	int tEmptyNum = 0;

	int nInsertedNum = 0;
	int nLeftPileNum = 0;
	for (unsigned int i = 0; i < mSize; ++i)
	{
		// 格子是空的
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			tEmpty[tEmptyNum] = i;
			tEmptyNum++;
			if (nPileLimit == 1 && tEmptyNum >= nItemNum)
			{
				break;
			}
		}
		else if (mItemID[i] == nItemID && nPileLimit > 1) // 在非空格子上插入
		{
			CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
			if (NULL != pItemObject)
			{
				nLeftPileNum = nPileLimit - pItemObject->GetItemNum();
				if (nLeftPileNum <= 0)
				{
					// 道具的堆叠数目出错了
					LOG_ERROR("item pile num valid");
					continue;
				}
				nInsertedNum = nItemNum > nLeftPileNum ? nLeftPileNum : nItemNum;
				pItemObject->SetItemNum(pItemObject->GetItemNum() + nInsertedNum);
				nItemNum -= nInsertedNum;

				// 记录插入在那个格子里
				pOutIndex[rOutLen] = i;
				// 对应的这个格子里插入了多少个数目的道具
				pOutNumber[rOutLen] = pItemObject->GetItemNum();
				++rOutLen;
				// 已经全部插入了，重置ID与数量的关系
				if (nItemNum <= 0)
				{
					return 0;
				}
			}
		}
	}


	// 遍历在空格子里插入的道具
	for (int i = 0; i < tEmptyNum; i++)
	{
		CItemObject* pItemObject = CItemFactory::createItem(nItemID);
		if (NULL == pItemObject)
		{
			// 从道具实体池创建道具实体失败
			LOG_ERROR("create item object from pool failure!!!");
			continue;
		}
		pOutIndex[rOutLen] = tEmpty[i];
		pOutNumber[rOutLen] = nPileLimit > nItemNum ? nItemNum : nPileLimit;
		++rOutLen;

		mItemObjID[tEmpty[i]] = pItemObject->getObjID();
		mItemID[tEmpty[i]] = nItemID;
		// 如果没有插入的比堆叠上限还大
		if (nItemNum > nPileLimit)
		{
			pItemObject->SetItemNum(nPileLimit);
			nItemNum -= nPileLimit;
		}
		else
		{
			// 将剩余下的放在该处
			pItemObject->SetItemNum(nItemNum);
			break;
		}

	}
	return 0;
}

// 背包里是否有足够数量的道具
bool CItemContainer::checkEnough(int nItemID, int nItemNum)
{
	for (unsigned int i = 0; i < mSize; ++i)
	{
		// 格子是空的
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			continue;
		}

		if (mItemID[i] != nItemID)
		{
			continue;
		}

		CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
		if (NULL == pItemObject)
		{
			continue;
		}
		nItemNum -= pItemObject->GetItemNum();
		if (nItemNum <= 0)
		{
			return true;
		}
	}

	return false;
}

/// 删除道具
void CItemContainer::removeItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen)
{
	for (unsigned int i = 0; i < mSize; ++i)
	{
		// 格子是空的
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			continue;
		}

		if (mItemID[i] != nItemID)
		{
			continue;
		}

		CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
		if (NULL == pItemObject)
		{
			continue;
		}
		
		// 当前的这个道具已经够了，不需要再往后找了
		if (nItemNum <= (int)pItemObject->GetItemNum())
		{
			pItemObject->SetItemNum(pItemObject->GetItemNum() - nItemNum);

			pOutIndex[rOutLen] = i;
			pOutNumber[rOutLen] = pItemObject->GetItemNum();
			++rOutLen;

			if (0 == pItemObject->GetItemNum())
			{
				mItemObjID[i] = INVALID_OBJ_ID;
				mItemID[i] = 0;
				CItemFactory::destroyItem(pItemObject->getObjID());
			}
			return;
		}
		else
		{
			mItemObjID[i] = INVALID_OBJ_ID;
			mItemID[i] = 0;

			pOutIndex[rOutLen] = i;
			pOutNumber[rOutLen] = 0;
			++rOutLen;

			CItemFactory::destroyItem(pItemObject->getObjID());
		}
	}

	return;
}

/// 删除道具
bool CItemContainer::removeItem(unsigned int nIndex, unsigned int nNum)
{
	if (nIndex >= mSize)
	{
		return false;
	}

	if (INVALID_OBJ_ID == mItemObjID[nIndex])
	{
		return false;
	}

	CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[nIndex]);
	if (NULL == pItemObject)
	{
		return false;
	}

	if (pItemObject->GetItemNum() < nNum)
	{
		return false;
	}

	pItemObject->SetItemNum(pItemObject->GetItemNum() - nNum);
	// 如果删除完了
	if (pItemObject->GetItemNum() <= 0)
	{
		mItemObjID[nIndex] = INVALID_OBJ_ID;
		mItemID[nIndex] = 0;
		CItemFactory::destroyItem(pItemObject->getObjID());
	}
	return true;
}