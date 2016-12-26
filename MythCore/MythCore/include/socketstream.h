#ifndef __SOCKETSTREAM_H__
#define __SOCKETSTREAM_H__
#include "commontype.h"
#define QUEUERESERVELENGTH 8	// Ԥ�����ֵĳ���

namespace Myth
{
	class CSocketStream
	{
	public:
		CSocketStream(void);
		CSocketStream(int nTotalSize);
		~CSocketStream(void);

	public:
		// ��ʼ��
		int			Initialize(uint8* pBuffer, int nTotalSize);
		// ����
		int			Resume(uint8* pBuffer, int nTotalSize);
		// ��һ�����ݸ����ڻ���������
		int			PushPacket(const uint8* pCode, int nLength);
		// �ӻ�����ȡ��һ����Ϣ
		int			GetHeadPacket(uint8* pCode, int &rLength);
		// �ӻ�����ȡ��һ����Ϣ
		int			PeekHeadPacket(uint8* pCode, int& rLength);
		// �ӻ�����ɾ��һ����Ϣ
		int			DeleteHeadPacket();
		// ��ָ��ƫ�ƴ�ȡ��һ����Ϣ
		int			GetOnePacket(int nOffset, int nLength, uint8* pOutCode, int& rOutLength);
		// ת�����ļ�
		int			DumpToFile(const char* pFileName);
		// ���ļ������
		int			LoadFromFile(const char* pFileName);
		// ��ն���
		void		ClearBuffer();
		// �Ƿ�Ϊ��
		bool		IsEmpty();
		// �õ�ʣ��ռ�
		sint32		GetLeftSpace();


		bool		IsFull();
		inline void			GetCriticalData(sint32 &rBegin, sint32 &rEnd)
		{
			rBegin = mBegin;
			rEnd = mEnd;
		}
		inline  void		SetCriticalData(sint32 nBegin, sint32 nEnd)
		{
			if (nBegin >= 0 && nBegin < mSize)
			{
				mBegin = nBegin;
			}

			if (nEnd >= 0 && nEnd < mSize)
			{
				mEnd = nEnd;
			}
		}

	private:
		sint32				mSize;
		sint32				mBegin;
		sint32				mEnd;
	};
}


#endif