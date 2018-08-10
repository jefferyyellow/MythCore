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
		int			Initialize(const byte* pBuffer, int nTotalSize);
		// ����
		int			Resume(const byte* pBuffer, int nTotalSize);
		// ��һ�����ݸ����ڻ���������
		int			PushPacket(const byte* pCode, int nLength);
		// �ӻ�����ȡ��һ����Ϣ
		int			GetHeadPacket(byte* pCode, int &rLength);
		// �ӻ�����ȡ��һ����Ϣ
		int			PeekHeadPacket(byte* pCode, int& rLength);
		// �ӻ�����ɾ��һ����Ϣ
		int			DeleteHeadPacket();
		// ��ָ��ƫ�ƴ�ȡ��һ����Ϣ
		int			GetOnePacket(int nOffset, int nLength, byte* pOutCode, int& rOutLength);
		// ת�����ļ�
		int			DumpToFile(const char* pFileName);
		// ���ļ������
		int			LoadFromFile(const char* pFileName);
		// ��ն���
		void		ClearBuffer();
		// �Ƿ�Ϊ��
		bool		IsEmpty();
		// �õ�ʣ��ռ�
		int			GetLeftSpace();


		bool		IsFull();
		inline void			GetCriticalData(int &rBegin, int &rEnd)
		{
			rBegin = mBegin;
			rEnd = mEnd;
		}
		inline  void		SetCriticalData(int nBegin, int nEnd)
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
		int				mSize;
		int				mBegin;
		int				mEnd;
	};
}


#endif