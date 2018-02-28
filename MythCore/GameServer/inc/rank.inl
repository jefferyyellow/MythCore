
template< int MaxRankNum, class T >
int CRank< MaxRankNum, T >::refreshRankValue(CRankValueType &rRankValue)
{
	int tRankIndex = -1;
	KEY_INDEX_MAP::iterator it = mMapKeyMemIndex.find(rRankValue.mRankKey);
	bool bUp = false;

	if (mMapKeyMemIndex.end() == it)
	{
		// 已经满了
		if (mSortRankNum >= MaxRankNum)
		{
			int nMemoryIndex = mSortRank[mSortRankNum - 1];
			CRankValueType *tpLastRankValue = mRankDataPool.get(nMemoryIndex);
			if (NULL != tpLastRankValue)
			{
				//rRankValue.mRankValue > tpLastRankValue->mRankValue )
				if (CompareRankValue(&rRankValue, tpLastRankValue))
				{
					mMapKeyMemIndex.erase(tpLastRankValue->mRankKey);

					KEY_INDEX_MAP::value_type valType(rRankValue.mRankKey, nMemoryIndex);
					mMapKeyMemIndex.insert(valType);

					*tpLastRankValue = rRankValue;
					tpLastRankValue->mRankIndex = mSortRankNum - 1;
					tRankIndex = mSortRankNum - 1;
				}
			}
		}
		else
		{
			int nMemoryIndex = -1;
			CRankValueType* pNewNode = mRankDataPool.allocate(nMemoryIndex);
			if (NULL == pNewNode)
			{
				return -1;
			}
			*pNewNode = rRankValue;
			pNewNode->mRankIndex = mSortRankNum;

			KEY_INDEX_MAP::value_type valType(rRankValue.mRankKey, nMemoryIndex);
			mMapKeyMemIndex.insert(valType);

			mSortRank[mSortRankNum] = nMemoryIndex;
			tRankIndex = mSortRankNum;
			// 增加排行榜数目
			++mSortRankNum;
		}

		if (tRankIndex == -1)
		{
			return -1;
		}
		bUp = true;
	}
	else
	{
		CRankValueType *tpPreValueType = mRankDataPool.get(it->second);
		if (NULL == tpPreValueType)
		{
			mMapKeyMemIndex.erase(rRankValue.mRankKey);
			return -1;
		}
		if (CompareRankValue(&rRankValue, tpPreValueType))
		{
			bUp = true;
		}
		tRankIndex = tpPreValueType->mRankIndex;
		*tpPreValueType = rRankValue;
		tpPreValueType->mRankIndex = tRankIndex;
	}

	// 上升
	int nIndex = -1;
	if (bUp)
	{
		CRankValueType* tpRankValueType = NULL;
		nIndex = upperBound(0, tRankIndex - 1, tRankIndex);
		if (nIndex < tRankIndex)
		{
			int nMemberIndex = mSortRank[tRankIndex];
			for (int i = tRankIndex - 1; i >= nIndex; -- i)
			{
				mSortRank[i + 1] = mSortRank[i];
				tpRankValueType = mRankDataPool.get(mSortRank[i]);
				if (NULL != tpRankValueType)
				{
					tpRankValueType->mRankIndex = i + 1;
				}
			}
			mSortRank[nIndex] = nMemberIndex;
			tpRankValueType = mRankDataPool.get(mSortRank[nIndex]);
			if (NULL != tpRankValueType)
			{
				tpRankValueType->mRankIndex = nIndex;
			}
		}
	}

	// 下降
	else
	{
		nIndex = upperBound(tRankIndex + 1, mSortRankNum - 1, tRankIndex);
		if (nIndex > tRankIndex)
		{
			CRankValueType* tpRankValueType = NULL;

			int nMemberIndex = mSortRank[tRankIndex];
			for (int i = nIndex; i < tRankIndex; --i)
			{
				mSortRank[i - 1] = mSortRank[i];
				tpRankValueType = mRankDataPool.get(mSortRank[i]);
				if (NULL != tpRankValueType)
				{
					tpRankValueType->mRankIndex = i - 1;
				}
			}
			mSortRank[nIndex] = nMemberIndex;
			tpRankValueType = mRankDataPool.get(mSortRank[nIndex]);
			if (NULL != tpRankValueType)
			{
				tpRankValueType->mRankIndex = nIndex;
			}
		}
	}

	return nIndex;
}

// 获取玩家排名
template< int MaxRankNum, class T  >
int CRank< MaxRankNum, T >::getRankIndexByKey(int nRankKey)
{
	KEY_INDEX_MAP::iterator it = mMapKeyMemIndex.find(nRankKey);
	if (mMapKeyMemIndex.end() == it)
	{
		return -1;
	}

	CRankValueType *tpValueType = mRankDataPool.get(it->second);
	if (NULL == tpValueType)
	{
		mMapKeyMemIndex.erase(nRankKey);
		return -1;
	}

	return tpValueType->mRankIndex;
}

// 根据索引获取某项排行榜玩家数据
template< int MaxRankNum, class T  >
int CRank< MaxRankNum, T >::getRankKeyByIndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= mSortRankNum)
	{
		return 0;
	}
	CRankValueType* tpRankValueType = mRankDataPool.get(mSortRank[nIndex]);
	if (NULL == tpRankValueType)
	{
		return 0;
	}

	return tpRankValueType->mRankKey;
}

template< int MaxRankNum, class T  >
bool CRank< MaxRankNum, T >::CompareRankValue(CRankValueType *pSrcValue, CRankValueType *pDesValue)
{
	if (NULL == pSrcValue || NULL == pDesValue)
	{
		return false;
	}
	if (pSrcValue->mRankValue > pDesValue->mRankValue)
	{
		return true;
	}
	if (pSrcValue->mRankValue == pDesValue->mRankValue)
	{
		// 时间小的优先
		if (pSrcValue->mRankTime < pDesValue->mRankTime)
		{
			return true;
		}
	}
	return false;
}

template< int MaxRankNum, class T  >
int	CRank< MaxRankNum, T >::upperBound(int nFirst, int nLast, int nIndex)
{
	int nCount = nLast - nFirst + 1;
	int nMiddle = 0;
	CRankValueType* tpIndexValue = mRankDataPool.get(mSortRank[nIndex]);
	if (NULL == tpIndexValue)
	{
		return nLast;
	}

	while (0 < nCount)
	{	
		nMiddle = nFirst + (nCount >> 1);
		CRankValueType* tpMiddleValue = mRankDataPool.get(mSortRank[nMiddle]);
		if (NULL == tpMiddleValue)
		{
			return nLast;
		}
		if (!CompareRankValue(tpIndexValue, tpMiddleValue))
		{
			nFirst = ++ nMiddle;
			nCount -= (nCount >> 1) + 1;
		}
		else
		{
			nCount = nCount >> 1;
		}
	}
	return nFirst;
}