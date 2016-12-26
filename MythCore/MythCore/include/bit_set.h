#ifndef __BIT_SET_H__
#define __BIT_SET_H__
#include "commontype.h"
/// sizeof(uint32)
#define	MYTH_BITLEN			(4 * 8)
// 32 = 2^5  ->   MYTH_BITLEN = 2 ^ MYTH_BITLEN_SHIFT
#define	MYTH_BITLEN_SHIFT	5				
#define BIT_SET_LEN(length) ((length+MYTH_BITLEN-1)/MYTH_BITLEN)

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
		void	clearBit(uint32 uIndex)
		{
			if (uIndex >= Capacity)
			{
				return;
			}

			uint32 mask = 1 << (uIndex & (MYTH_BITLEN - 1));
			mBitSet[uIndex >> MYTH_BITLEN_SHIFT] &= ~mask;
		}
		/// check the uIndex bit is set
		bool	getBit(uint uIndex) const
		{
			if (uIndex >= Capacity)
			{
				return false;
			}
			uint32 mask = 1 << (uIndex & (MYTH_BITLEN - 1));
			return 0 != (mBitSet[uIndex >> MYTH_BITLEN_SHIFT] & mask);
		}
		/// set the uIndex bit to 1
		void	setBit(uint uIndex)
		{
			if (uIndex >= Capacity)
			{
				return;
			}
			uint32 mask = 1 << (uIndex & (MYTH_BITLEN - 1));
			mBitSet[uIndex >> MYTH_BITLEN_SHIFT] |= mask;
		}
		/// check the uIndex bit is set to 1
		bool	operator[](uint uIndex) const
		{
			return getBit(uIndex);
		}
		/// set all bit to 1
		void	setAllBit()
		{
			memset(mBitSet, ~((uint)0), sizeof(mBitSet));
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
		uint32	getBitSet(uint uIndex)
		{
			if (uIndex >= mBitSetSize)
			{
				return 0;
			}

			return mBitSet[uIndex];
		}

		/// set function for mBitSet
		void	setBitSet(uint uIndex, uint32 uBitSet)
		{
			if (uIndex >= mBitSetSize)
			{
				return;
			}

			mBitSet[uIndex] = uBitSet;
		}

	private:
		/// calculate the mLastMask
		void	caluLastMask()
		{
			uint uLastBits = Capacity & (MYTH_BITLEN - 1);
			// if Capacity % MYTH_BITLEN == 0
			if (uLastBits == 0)
			{
				mLastMask = ~((uint)0);
			}
			else
			{
				mLastMask = (1 << uLastBits) - 1;
			}
		}

	private:
		/// bit array
		uint32	mBitSet[BIT_SET_LEN(Capacity)];
		/// size of array mBitSet
		uint32	mBitSetSize;
		/// the last uint32 mask
		uint32	mLastMask;
	};
}

#include "bit_set.inl"
#endif