#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__
#include "commontype.h"

namespace Myth
{
	class CShareMemory
	{
	public:
		static byte*		createShareMemory(int nKey, int nSize, bool& bCreate);
		static int			destroyShareMemory(int nKey);

	public:
		int			getShmKey(){ return mShmKey; }
		void		setShmKey(int nShmKey){ mShmKey = nShmKey; }

		int			getShmSize(){ return mShmSize; }
		void		setShmSize(int nShmSize){ mShmSize = nShmSize; }

		byte*		getShmPoint(){ return mpShmPoint; }
		void		setShmPoint(byte* pShmPoint){ mpShmPoint = pShmPoint; }

	private:
		/// 共享内存的键值
		int			mShmKey;
		/// 共享内存的大小
		int			mShmSize;
		/// 共享内存指针
		byte*		mpShmPoint;
	};
}

#endif