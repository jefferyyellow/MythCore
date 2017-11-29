#include "bytestream.h"
#include <string.h>
namespace Myth
{
	CByteStream::CByteStream(void)
	{
		mSize = 0;
		mBegin = 0;
		mEnd = 0;
	}

	CByteStream::~CByteStream(void)
	{
	}


	// ��ʼ��
	int CByteStream::Initialize(byte* pBuffer, int nTotalSize)
	{
		if (NULL == pBuffer)
		{
			return -1;
		}
		mBuffer = pBuffer;
		mSize = nTotalSize;
		mBegin = 0;
		mEnd = 0;
		return 0;
	}

	// ����
	int	CByteStream::Resume(byte* pBuffer, int nTotalSize)
	{
		if (NULL == pBuffer)
		{
			return -1;
		}
		mBuffer = pBuffer;
		mSize = nTotalSize;
		return 0;
	}

	// ��һ�����ݸ����ڻ���������
	int	CByteStream::PushPacket(const byte* pCode, int nLength)
	{
		// ����У��
		if (NULL == pCode || nLength <= 0)
		{
			return -1;
		}


		int nLeftSpace = 0;
		int nBegin = -1;
		int nEnd = -1;

		if (IsFull())
		{
			return -2;
		}

		GetCriticalData(nBegin, nEnd);
		if (nBegin < 0 || nBegin >= mSize || nEnd < 0 || nEnd >= mSize)
		{
			ClearBuffer();
			return -3;
		}
		nLeftSpace = GetLeftSpace();

		if ((int)(nLength + sizeof(short)) > nLeftSpace || nLength > 0xFFFF)
		{
			return -4;
		}

		byte* pBuffer = mBuffer;


		unsigned short usLength = (unsigned short)nLength;
		byte* pSrc = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(unsigned short); ++i)
		{
			pBuffer[nEnd] = pSrc[i];
			// ע������Ļ�ת
			nEnd = (nEnd + 1) % mSize;
		}

		//							   ��β	 ��ͷ
		// ��ת�ķ�ʽ�ˣ����·�ʽ |���ݣ����������|�����ಿ�����м�
		if (nBegin > nEnd)
		{
			memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)nLength);
		}
		else
		{
			//						   ��ͷ  ��β
			// ������ʽ,���·�ʽ ����������ݣ������

			// �󲿵Ŀ��ಿ��д���£�����дһ����ǰ��Ŀ��ಿ��
			if ((int)nLength > (mSize - nEnd))
			{
				memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)(mSize - nEnd));
				memcpy((void*)pBuffer, (const void *)&pCode[mSize - nEnd], (size_t)(nLength + nEnd - mSize));
			}
			else
			{
				// ����Ŀ��ಿ��д����
				memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)nLength);
			}
		}
		// ����д���ݺ��β��ƫ��
		nEnd = (nEnd + nLength) % mSize;
		SetCriticalData(-1, nEnd);
		return 0;
	}


	// �ӻ�����ȡ��һ����Ϣ
	int	CByteStream::GetHeadPacket(byte* pCode, int &rLength)
	{
		// ����У��
		if (NULL == pCode)
		{
			rLength = 0;
			return -1;
		}


		int nBegin = -1;
		int nEnd = -1;

		if (IsEmpty())
		{
			rLength = 0;
			return -2;
		}

		GetCriticalData(nBegin, nEnd);
		// �õ����ݳ���
		int nDataLength = 0;
		if (nBegin < nEnd)
		{
			nDataLength = nEnd - nBegin;
		}
		else
		{
			nDataLength = mSize - nBegin + nEnd;
		}
		if (nDataLength < (int)sizeof(short))
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -3;
		}

		byte* pBuffer = mBuffer;

		// ��ȡ��Ϣ������(ע����PushPacket��д��Ϣ�����ȶ�Ӧ)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// ��Ϣ�Ƿ��Ѿ���ȫ��
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -4;
		}

		rLength = usLength;

		//						   ��ͷ  ��β
		// ������ʽ,���·�ʽ ����������ݣ������,������һ�𣬲�����ͷ���еĻ���һ���Ծ��ܿ�����
		if (nBegin < nEnd)
		{
			memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(usLength));
		}
		else
		{
			//							   ��β	 ��ͷ
			// ��ת�ķ�ʽ�ˣ����·�ʽ |���ݣ����������|�����ಿ�����м�

			//������ַ�Ƭ����ֶο���
			if (mSize - nBegin < usLength)
			{
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(mSize - nBegin));
				memcpy((void*)(&pCode[mSize - nBegin]), (const void*)pBuffer, (size_t)(usLength + nBegin - mSize));
			}
			else
			{
				// ��Ȼ��������ͷ���У�������ο���ֻ��Ҫ����һͷ��
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(usLength));
			}
		}

		nBegin = (nBegin + usLength) % mSize;
		SetCriticalData(nBegin, -1);
		return 0;
	}

	// // �ӻ�����ȡ��һ����Ϣ��ע���GetHeadCode����,�ú���ȡ�����ݺ󲢲������ٽ���ƫ��
	int CByteStream::PeekHeadPacket(byte* pCode, int& rLength)
	{
		// ����У��
		if (NULL == pCode)
		{
			return -1;
		}

		int nBegin = -1;
		int nEnd = -1;

		if (IsEmpty())
		{
			rLength = 0;
			return -2;
		}

		GetCriticalData(nBegin, nEnd);
		// �õ����ݳ���
		int nDataLength = 0;
		if (nBegin < nEnd)
		{
			nDataLength = nEnd - nBegin;
		}
		else
		{
			nDataLength = mSize - nBegin + nEnd;
		}
		if (nDataLength < (int)sizeof(short))
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -3;
		}

		byte* pBuffer = mBuffer;

		// ��ȡ��Ϣ������(ע����PushPacket��д��Ϣ�����ȶ�Ӧ)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// ��Ϣ�Ƿ��Ѿ���ȫ��
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -4;
		}

		rLength = usLength;

		//						   ��ͷ  ��β
		// ������ʽ,���·�ʽ ����������ݣ������,������һ�𣬲�����ͷ���еĻ���һ���Ծ��ܿ�����
		if (nBegin < nEnd)
		{
			memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(&usLength));
		}
		else
		{
			//							   ��β	 ��ͷ
			// ��ת�ķ�ʽ�ˣ����·�ʽ |���ݣ����������|�����ಿ�����м�

			//������ַ�Ƭ����ֶο���
			if (mSize - nBegin < usLength)
			{
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(mSize - nBegin));
				memcpy((void*)(pCode[mSize - nBegin]), (const void*)&pBuffer, (size_t)(usLength + nBegin - mSize));
			}
			else
			{
				// ��Ȼ��������ͷ���У�������ο���ֻ��Ҫ����һͷ��
				memcpy((void*)pDest, (const void*)&pBuffer[nBegin], (size_t)(&usLength));
			}
		}

		nBegin = (nBegin + usLength) % mSize;
		return 0;
	}

	// �ӻ�����ɾ��һ����Ϣ
	int	CByteStream::DeleteHeadPacket()
	{
		int nBegin = -1;
		int nEnd = -1;

		if (IsEmpty())
		{
			return -1;
		}

		GetCriticalData(nBegin, nEnd);
		// �õ����ݳ���
		int nDataLength = 0;
		if (nBegin < nEnd)
		{
			nDataLength = nEnd - nBegin;
		}
		else
		{
			nDataLength = mSize - nBegin + nEnd;
		}
		if (nDataLength < (int)sizeof(short))
		{
			SetCriticalData(nEnd, -1);
			return -2;
		}
		byte* pBuffer = mBuffer;

		// ��ȡ��Ϣ������(ע����PushPacket��д��Ϣ�����ȶ�Ӧ)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// ��Ϣ�Ƿ��Ѿ���ȫ��
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			SetCriticalData(nEnd, -1);
			return -4;
		}

		nBegin = (nBegin + usLength) % mSize;
		SetCriticalData(nBegin, -1);
		return 0;
	}

	// ��ָ��ƫ�ƴ�ȡ��һ����Ϣ
	int	CByteStream::GetOnePacket(int nOffset, int nLength, byte* pOutCode, int& rOutLength)
	{
		// ����У��
		if (NULL == pOutCode || nLength <= 0)
		{
			return -1;
		}


		if (nOffset < 0 || nOffset >= mSize)
		{
			rOutLength = 0;
			return -2;
		}

		if (nLength < 0 || nLength >= mSize)
		{
			return -3;
		}



		if (IsEmpty())
		{
			rOutLength = 0;
			return -4;
		}

		// �õ��ٽ紦������
		int nBegin = -1;
		int nEnd = -1;
		GetCriticalData(nBegin, nEnd);

		if (nOffset < nBegin || nOffset >= nEnd)
		{
			rOutLength = 0;
			return -5;
		}

		// �õ����ݳ���
		int nDataLength = 0;
		if (nBegin < nEnd)
		{
			nDataLength = nEnd - nBegin;
		}
		else
		{
			nDataLength = mSize - nBegin + nEnd;
		}
		if (nDataLength < (int)sizeof(short))
		{
			SetCriticalData(nEnd, -1);
			return -6;
		}
		byte* pBuffer = mBuffer;

		// ��ȡ��Ϣ������(ע����PushPacket��д��Ϣ�����ȶ�Ӧ)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}
		// ��Ϣ�Ƿ��Ѿ���ȫ��
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0 || nLength != usLength)
		{
			rOutLength = 0;
			return -7;
		}

		SetCriticalData(nOffset, -1);
		return GetHeadPacket(pOutCode, rOutLength);
	}



	// ת�����ļ�
	int CByteStream::DumpToFile(const char* pFileName)
	{
		return 0;
	}

	// ���ļ������
	int	CByteStream::LoadFromFile(const char* pFileName)
	{
		return 0;
	}

	// ��ն���
	void	CByteStream::ClearBuffer()
	{
		mBegin = mEnd = 0;
	}

	// �õ�ʣ��ռ�
	int	CByteStream::GetLeftSpace()
	{
		int nMaxLength = 0;
		int nBegin = -1;
		int nEnd = -1;

		GetCriticalData(nBegin, nEnd);
		if (nBegin == nEnd)
		{
			nMaxLength = mSize;
		}
		else if (nBegin > nEnd)
		{
			//							   ��β	 ��ͷ
			// ��ת�ķ�ʽ�ˣ����·�ʽ |���ݣ����������|
			nMaxLength = nBegin - nEnd;
		}
		else
		{
			//						   ��ͷ  ��β
			// ������ʽ,���·�ʽ ����������ݣ������
			nMaxLength = (mSize - nEnd) + nBegin;
		}

		nMaxLength -= QUEUERESERVELENGTH;
		return nMaxLength;
	}

	// �����Ƿ�Ϊ��
	bool CByteStream::IsEmpty()
	{
		int nBegin = -1;
		int nEnd = -1;

		GetCriticalData(nBegin, nEnd);
		if (nBegin == nEnd)
		{
			return true;
		}

		return false;
	}

	bool CByteStream::IsFull()
	{
		// ����ռ�Ĵ�С
		int nMaxLength = GetLeftSpace();
		if (nMaxLength > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
