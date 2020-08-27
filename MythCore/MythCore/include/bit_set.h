#ifndef __BIT_SET_H__
#define __BIT_SET_H__
#include "commontype.h"

namespace Myth
{
	template<int Capacity>
	class CBitSet
	{
	public:
		/// \name Object.
		//@{
		CBitSet()
		{
			init(false);
		}

		CBitSet(bool bAllSet)
		{
			init(bAllSet);
		}

		CBitSet(const CBitSet& rOther)
		{
			memcpy(mBitSet, rOther.mBitSet, sizeof(mBitSet));
			mBitSetSize = rOther.mBitSetSize;
			mLastMask = rOther.mLastMask;
		}

		~CBitSet()
		{
		}
		CBitSet	&operator=(const CBitSet &rOther)
		{
			memcpy(mBitSet, rOther.mBitSet, sizeof(mBitSet));
			mBitSetSize = rOther.mBitSetSize;
			mLastMask = rOther.mLastMask;
			return *this;
		}
		void init(bool bAllSet);
		void final()
		{
		}
		//@}

	public:
		/// get the size of used array
		int		size() const{ return Capacity; }
		/// clear the uIndex bit
		void	clearBit(unsigned int uIndex)
		{
			if (uIndex >= Capacity)
			{
				return;
			}

			int mask = 1 << (uIndex & (INT_BIT_NUM - 1));
			mBitSet[uIndex >> INT_BIT_LEN_SHIFT] &= ~mask;
		}
		/// check the uIndex bit is set
		bool	getBit(unsigned int uIndex) const
		{
			if (uIndex >= Capacity)
			{
				return false;
			}
			int mask = 1 << (uIndex & (INT_BIT_NUM - 1));
			return 0 != (mBitSet[uIndex >> INT_BIT_LEN_SHIFT] & mask);
		}
		/// set the uIndex bit to 1
		void	setBit(unsigned int uIndex)
		{
			if (uIndex >= Capacity)
			{
				return;
			}
			int mask = 1 << (uIndex & (INT_BIT_NUM - 1));
			mBitSet[uIndex >> INT_BIT_LEN_SHIFT] |= mask;
		}
		/// check the uIndex bit is set to 1
		bool	operator[](unsigned int uIndex) const
		{
			return getBit(uIndex);
		}
		/// set all bit to 1
		void	setAllBit()
		{
			memset(mBitSet, ~((int)0), sizeof(mBitSet));
			mBitSet[mBitSetSize - 1] &= mLastMask;
		}
		/// set all bit to 0
		void	clearAllBit()
		{
			memset(mBitSet, 0, sizeof(mBitSet));
		}
		/// check all bit already set to 1
		bool	checkAllBitSet();
		/// check all bit already set 0
		bool	checkAllBitClear();

	public:
		/// \name Bit operations.
		//@{
		/// return not with this
		CBitSet	operator~() const;
		/// return this and with rOther
		CBitSet	operator&(const CBitSet &rOther) const;
		/// return this or with rOther
		CBitSet	operator|(const CBitSet &rOther) const;
		/// return this xor with rOther
		CBitSet operator^(const CBitSet &rOther) const;

		/// the bit array and with rOther and return this
		CBitSet& operator&=(const CBitSet &rOther);
		/// the bit array or with rOther and return this
		CBitSet& operator|=(const CBitSet &rOther);
		/// the bit array xor with rOther and return this
		CBitSet& operator^=(const CBitSet &rOther);

		/// check the bit array equip to rOther
		bool	operator==(const CBitSet &rOther);
		/// check the bit array not equip to rOther
		bool	operator!=(const CBitSet &rOther);
		//@}

	public:
		/// get function for mBitSet
		int	getBitSet(unsigned int uIndex)
		{
			if (uIndex >= (unsigned int)mBitSetSize)
			{
				return 0;
			}

			return mBitSet[uIndex];
		}

		/// set function for mBitSet
		void	setBitSet(unsigned int uIndex, int uBitSet)
		{
			if (uIndex >= (unsigned int)mBitSetSize)
			{
				return;
			}

			mBitSet[uIndex] = uBitSet;
		}

		/// get size function for mBitSet
		int		getBitSetSize()
		{
			return mBitSetSize;
		}

	private:
		/// calculate the mLastMask
		void	caluLastMask()
		{
			int uLastBits = Capacity & (INT_BIT_NUM - 1);
			// if Capacity % MYTH_BITLEN == 0
			if (uLastBits == 0)
			{
				mLastMask = ~((int)0);
			}
			else
			{
				mLastMask = (1 << uLastBits) - 1;
			}
		}

	private:
		/// bit array
		int	mBitSet[BIT_SET_LEN(Capacity)];
		/// size of array mBitSet
		int	mBitSetSize;
		/// the last int mask
		int	mLastMask;
	};
}

#include "bit_set.inl"
#endif