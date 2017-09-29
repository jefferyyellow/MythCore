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

	// ���������Ŀ�Ͷѵ���Ŀ
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

	// �Ƚ����Զѵ��ĵ��߷��ڶ�Ӧ���Ѿ��еĸ�������
	int nTotalSpace = 0;
	for (int i = 0; i < (int)mSize; ++ i)
	{
		// �ո���
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nTotalSpace += 1;
		}
		else
		{
			// �Ѿ��е��ߵĸ����Ƿ��ܷ�����һ�Ѷ����������һ�ֻ��еĿ��Զѵ��ĵ���
			// ������ԣ����ٶѵ�����
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

	// �������жѵ���ĿΪ1�ģ���û�з��µĶѵ���Ŀ����1�ĵ���
	// ��������Ҫ�Ŀո�������
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

// �����һ�ֵ��ߣ�����ܶ࣬�ո��ӿ���ֱ���ã����ÿ����ȶѵ�
// ������ʹ�ÿո���
bool CItemContainer::checkSpace(int nItemID, int nNumber)
{
	CTplItem* pTplItem = (CTplItem*)CStaticData::searchTpl(nItemID);
	if (NULL == pTplItem)
	{
		return true;
	}

	for (int i = 0; i < (int)mSize; ++ i)
	{
		// ֱ���ÿո���
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			nNumber -= pTplItem->mPileLimit;
		}
		// �ѵ�
		else if (nItemID == mItemID[i])
		{
			CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
			if (NULL != pItemObject)
			{
				nNumber -= pTplItem->mPileLimit - pItemObject->GetItemNum();
			}
		}
		// ��⵽���Է�����
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

	// �������ģ���Ҳ���
	CTplItem* tpItem = (CTplItem*)CStaticData::searchTpl(nItemID);
	if (NULL == tpItem)
	{
		// ����ģ������Ϊ��
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
		// �����ǿյ�
		if (INVALID_OBJ_ID == mItemObjID[i])
		{
			tEmpty[tEmptyNum] = i;
			tEmptyNum++;
			if (nPileLimit == 1 && tEmptyNum >= nItemNum)
			{
				break;
			}
		}
		else if (mItemID[i] == nItemID && nPileLimit > 1) // �ڷǿո����ϲ���
		{
			CItemObject* pItemObject = (CItemObject*)CObjPool::Inst()->getObj(mItemObjID[i]);
			if (NULL != pItemObject)
			{
				nLeftPileNum = nPileLimit - pItemObject->GetItemNum();
				if (nLeftPileNum <= 0)
				{
					// ���ߵĶѵ���Ŀ������
					LOG_ERROR("item pile num valid");
					continue;
				}
				nInsertedNum = nItemNum > nLeftPileNum ? nLeftPileNum : nItemNum;
				pItemObject->SetItemNum(pItemObject->GetItemNum() + nInsertedNum);
				nItemNum -= nInsertedNum;

				// ��¼�������Ǹ�������
				pOutIndex[rOutLen] = i;
				// ��Ӧ���������������˶��ٸ���Ŀ�ĵ���
				pOutNumber[rOutLen] = pItemObject->GetItemNum();
				++rOutLen;
				// �Ѿ�ȫ�������ˣ�����ID�������Ĺ�ϵ
				if (nItemNum <= 0)
				{
					return 0;
				}
			}
		}
	}


	// �����ڿո��������ĵ���
	for (int i = 0; i < tEmptyNum; i++)
	{
		CItemObject* pItemObject = CItemFactory::createItem(nItemID);
		if (NULL == pItemObject)
		{
			// �ӵ���ʵ��ش�������ʵ��ʧ��
			LOG_ERROR("create item object from pool failure!!!");
			continue;
		}
		pOutIndex[rOutLen] = tEmpty[i];
		pOutNumber[rOutLen] = nPileLimit > nItemNum ? nItemNum : nPileLimit;
		++rOutLen;

		mItemObjID[tEmpty[i]] = pItemObject->getObjID();
		mItemID[tEmpty[i]] = nItemID;
		// ���û�в���ıȶѵ����޻���
		if (nItemNum > nPileLimit)
		{
			pItemObject->SetItemNum(nPileLimit);
			nItemNum -= nPileLimit;
		}
		else
		{
			// ��ʣ���µķ��ڸô�
			pItemObject->SetItemNum(nItemNum);
			break;
		}

	}
	return 0;
}

// �������Ƿ����㹻�����ĵ���
bool CItemContainer::checkEnough(int nItemID, int nItemNum)
{
	for (unsigned int i = 0; i < mSize; ++i)
	{
		// �����ǿյ�
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

/// ɾ������
void CItemContainer::removeItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen)
{
	for (unsigned int i = 0; i < mSize; ++i)
	{
		// �����ǿյ�
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
		
		// ��ǰ����������Ѿ����ˣ�����Ҫ����������
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

/// ɾ������
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
	// ���ɾ������
	if (pItemObject->GetItemNum() <= 0)
	{
		mItemObjID[nIndex] = INVALID_OBJ_ID;
		mItemID[nIndex] = 0;
		CItemFactory::destroyItem(pItemObject->getObjID());
	}
	return true;
}