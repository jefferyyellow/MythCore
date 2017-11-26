#ifndef __BIT_SET_INL__
#define __BIT_SET_INL__

namespace Myth
{
	template<int Capacity>
	void CBitSet<Capacity>::init(bool bAllSet)
	{
		mBitSetSize = BIT_SET_LEN(Capacity);
		caluLastMask();
		if (bAllSet)
		{
			memset(mBitSet, ~((int)0), sizeof(mBitSet));
			mBitSet[mBitSetSize - 1] &= mLastMask;
		}
		else
		{
			memset(mBitSet, 0, sizeof(mBitSet));
		}
	}

	template<int Capacity>
	bool CBitSet<Capacity>::checkAllBitSet()
	{
		for (int i = 0; i < mBitSetSize - 1; ++i)
		{
			if (mBitSet[i] != ~(int(0)))
			{
				return false;
			}
		}

		if (mBitSet[mBitSetSize - 1] != mLastMask)
		{
			return false;
		}
		return true;
	}

	template<int Capacity>
	bool CBitSet<Capacity>::checkAllBitClear()
	{
		for (int i = 0; i < mBitSetSize; ++i)
		{
			if (mBitSet[i] != 0)
			{
				return false;
			}
		}

		return true;
	}

	template<int Capacity>
	CBitSet<Capacity> CBitSet<Capacity>::operator~() const
	{
		CBitSet<Capacity> tTemp = *this;
		for (int i = 0; i < tTemp.mBitSetSize; ++i)
		{
			tTemp.mBitSet[i] = ~tTemp.mBitSet[i];
		}

		tTemp.mBitSet[mBitSetSize - 1] &= tTemp.mLastMask;

		return tTemp;
	}

	template<int Capacity>
	CBitSet<Capacity> CBitSet<Capacity>::operator&(const CBitSet &rOther) const
	{
		CBitSet<Capacity> tTemp = *this;
		tTemp &= rOther;
		return tTemp;
	}

	template<int Capacity>
	CBitSet<Capacity> CBitSet<Capacity>::operator|(const CBitSet &rOther) const
	{
		CBitSet<Capacity> tTemp = *this;
		tTemp |= rOther;
		return tTemp;
	}

	template<int Capacity>
	CBitSet<Capacity> CBitSet<Capacity>::operator^(const CBitSet &rOther) const
	{
		CBitSet<Capacity> tTemp = *this;
		tTemp ^= rOther;
		return tTemp;
	}

	template<int Capacity>
	CBitSet<Capacity>& CBitSet<Capacity>::operator&=(const CBitSet &rOther)
	{
		for (uint i = 0; i < mBitSetSize; ++i)
		{
			mBitSet[i] &= rOther.mBitSet[i];
		}

		mBitSet[mBitSetSize - 1] &= mLastMask;
		return *this;
	}

	template<int Capacity>
	CBitSet<Capacity>& CBitSet<Capacity>::operator|=(const CBitSet &rOther)
	{
		for (uint i = 0; i < mBitSetSize; ++i)
		{
			mBitSet[i] |= rOther.mBitSet[i];
		}

		mBitSet[mBitSetSize - 1] &= mLastMask;
		return *this;
	}

	template<int Capacity>
	CBitSet<Capacity>& CBitSet<Capacity>::operator^=(const CBitSet &rOther)
	{
		for (uint i = 0; i < mBitSetSize; ++i)
		{
			mBitSet[i] ^= rOther.mBitSet[i];
		}
		mBitSet[mBitSetSize - 1] &= mLastMask;
		return *this;
	}

	template<int Capacity>
	bool CBitSet<Capacity>::operator==(const CBitSet &rOther)
	{
		for (uint i = 0; i < mBitSetSize; ++i)
		{
			if (mBitSet[i] != rOther.mBitSet[i])
			{
				return false;
			}
		}

		return true;
	}

	template<int Capacity>
	bool CBitSet<Capacity>::operator!=(const CBitSet &rOther)
	{
		return !(operator==(rOther));
	}
}

#endif