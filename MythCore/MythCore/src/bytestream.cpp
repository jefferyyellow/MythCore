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


	// 初始化
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

	// 重置
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

	// 将一个数据附加在缓冲区后面
	int	CByteStream::PushPacket(const byte* pCode, int nLength)
	{
		// 参数校验
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
			// 注意下面的回转
			nEnd = (nEnd + 1) % mSize;
		}

		//							   ｜尾	 ｜头
		// 回转的方式了，如下方式 |数据｜空余｜数据|，空余部分在中间
		if (nBegin > nEnd)
		{
			memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)nLength);
		}
		else
		{
			//						   ｜头  ｜尾
			// 正常方式,如下方式 ｜空余｜数据｜空余｜

			// 后部的空余部分写不下，还得写一部分前面的空余部分
			if ((int)nLength > (mSize - nEnd))
			{
				memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)(mSize - nEnd));
				memcpy((void*)pBuffer, (const void *)&pCode[mSize - nEnd], (size_t)(nLength + nEnd - mSize));
			}
			else
			{
				// 后面的空余部分写下了
				memcpy((void*)&pBuffer[nEnd], (const void*)pCode, (size_t)nLength);
			}
		}
		// 计算写数据后的尾部偏移
		nEnd = (nEnd + nLength) % mSize;
		SetCriticalData(-1, nEnd);
		return 0;
	}


	// 从缓冲区取出一个消息
	int	CByteStream::GetHeadPacket(byte* pCode, int &rLength)
	{
		// 参数校验
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
		// 得到数据长度
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

		// 读取消息包长度(注意与PushPacket中写消息包长度对应)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// 消息是否已经收全了
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -4;
		}

		rLength = usLength;

		//						   ｜头  ｜尾
		// 正常方式,如下方式 ｜空余｜数据｜空余｜,数据在一起，不是两头都有的话，一次性就能拷贝完
		if (nBegin < nEnd)
		{
			memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(usLength));
		}
		else
		{
			//							   ｜尾	 ｜头
			// 回转的方式了，如下方式 |数据｜空余｜数据|，空余部分在中间

			//如果出现分片，则分段拷贝
			if (mSize - nBegin < usLength)
			{
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(mSize - nBegin));
				memcpy((void*)(&pCode[mSize - nBegin]), (const void*)pBuffer, (size_t)(usLength + nBegin - mSize));
			}
			else
			{
				// 虽然数据在两头都有，但是这次拷贝只需要拷贝一头的
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(usLength));
			}
		}

		nBegin = (nBegin + usLength) % mSize;
		SetCriticalData(nBegin, -1);
		return 0;
	}

	// // 从缓冲区取出一个消息，注意和GetHeadCode区别,该函数取出数据后并不设置临界区偏移
	int CByteStream::PeekHeadPacket(byte* pCode, int& rLength)
	{
		// 参数校验
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
		// 得到数据长度
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

		// 读取消息包长度(注意与PushPacket中写消息包长度对应)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// 消息是否已经收全了
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			rLength = 0;
			SetCriticalData(nEnd, -1);
			return -4;
		}

		rLength = usLength;

		//						   ｜头  ｜尾
		// 正常方式,如下方式 ｜空余｜数据｜空余｜,数据在一起，不是两头都有的话，一次性就能拷贝完
		if (nBegin < nEnd)
		{
			memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(&usLength));
		}
		else
		{
			//							   ｜尾	 ｜头
			// 回转的方式了，如下方式 |数据｜空余｜数据|，空余部分在中间

			//如果出现分片，则分段拷贝
			if (mSize - nBegin < usLength)
			{
				memcpy((void*)pCode, (const void*)&pBuffer[nBegin], (size_t)(mSize - nBegin));
				memcpy((void*)(pCode[mSize - nBegin]), (const void*)&pBuffer, (size_t)(usLength + nBegin - mSize));
			}
			else
			{
				// 虽然数据在两头都有，但是这次拷贝只需要拷贝一头的
				memcpy((void*)pDest, (const void*)&pBuffer[nBegin], (size_t)(&usLength));
			}
		}

		nBegin = (nBegin + usLength) % mSize;
		return 0;
	}

	// 从缓冲区删除一个消息
	int	CByteStream::DeleteHeadPacket()
	{
		int nBegin = -1;
		int nEnd = -1;

		if (IsEmpty())
		{
			return -1;
		}

		GetCriticalData(nBegin, nEnd);
		// 得到数据长度
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

		// 读取消息包长度(注意与PushPacket中写消息包长度对应)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}

		// 消息是否已经收全了
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0)
		{
			SetCriticalData(nEnd, -1);
			return -4;
		}

		nBegin = (nBegin + usLength) % mSize;
		SetCriticalData(nBegin, -1);
		return 0;
	}

	// 从指定偏移处取出一个消息
	int	CByteStream::GetOnePacket(int nOffset, int nLength, byte* pOutCode, int& rOutLength)
	{
		// 参数校验
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

		// 得到临界处的数据
		int nBegin = -1;
		int nEnd = -1;
		GetCriticalData(nBegin, nEnd);

		if (nOffset < nBegin || nOffset >= nEnd)
		{
			rOutLength = 0;
			return -5;
		}

		// 得到数据长度
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

		// 读取消息包长度(注意与PushPacket中写消息包长度对应)
		unsigned short usLength = 0;
		byte* pDest = (byte*)&usLength;
		for (unsigned int i = 0; i < sizeof(short); ++i)
		{
			pDest[i] = pBuffer[nBegin];
			nBegin = (nBegin + 1) % mSize;
		}
		// 消息是否已经收全了
		if (usLength >(int)(nDataLength - sizeof(short)) || usLength <= 0 || nLength != usLength)
		{
			rOutLength = 0;
			return -7;
		}

		SetCriticalData(nOffset, -1);
		return GetHeadPacket(pOutCode, rOutLength);
	}



	// 转储到文件
	int CByteStream::DumpToFile(const char* pFileName)
	{
		return 0;
	}

	// 从文件里加载
	int	CByteStream::LoadFromFile(const char* pFileName)
	{
		return 0;
	}

	// 清空队列
	void	CByteStream::ClearBuffer()
	{
		mBegin = mEnd = 0;
	}

	// 得到剩余空间
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
			//							   ｜尾	 ｜头
			// 回转的方式了，如下方式 |数据｜空余｜数据|
			nMaxLength = nBegin - nEnd;
		}
		else
		{
			//						   ｜头  ｜尾
			// 正常方式,如下方式 ｜空余｜数据｜空余｜
			nMaxLength = (mSize - nEnd) + nBegin;
		}

		nMaxLength -= QUEUERESERVELENGTH;
		return nMaxLength;
	}

	// 队列是否为空
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
		// 空余空间的大小
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
