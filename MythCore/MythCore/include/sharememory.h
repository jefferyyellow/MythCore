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
		/// �����ڴ�ļ�ֵ
		int			mShmKey;
		/// �����ڴ�Ĵ�С
		int			mShmSize;
		/// �����ڴ�ָ��
		byte*		mpShmPoint;
	};
}

#endif