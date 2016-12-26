#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__
#include "commontype.h"

namespace Myth
{
	class CShareMemory
	{
	public:
		static uint8*		createShareMemory(uint32 nKey, int nSize, bool& bCreate);
		static int			destroyShareMemory(uint32 nKey, uint8* pShmPoint);

	public:
		uint32		getShmKey(){ return mShmKey; }
		void		setShmKey(uint32 nShmKey){ mShmKey = nShmKey; }

		uint32		getShmSize(){ return mShmSize; }
		void		setShmSize(uint32 nShmSize){ mShmSize = nShmSize; }

		uint8*		getShmPoint(){ return mpShmPoint; }
		void		setShmPoint(uint8* pShmPoint){ mpShmPoint = pShmPoint; }

	private:
		/// �����ڴ�ļ�ֵ
		uint32		mShmKey;
		/// �����ڴ�Ĵ�С
		uint32		mShmSize;
		/// �����ڴ�ָ��
		uint8*		mpShmPoint;
	};
}

#endif