#ifndef __SOCKETSTREAM_H__
#define __SOCKETSTREAM_H__
#include "commontype.h"
#define QUEUERESERVELENGTH 8	// 预留部分的长度

namespace Myth
{
	class CSocketStream
	{
	public:
		CSocketStream(void);
		CSocketStream(int nTotalSize);
		~CSocketStream(void);

	public:
		// 初始化
		int			Initialize(uint8* pBuffer, int nTotalSize);
		// 重置
		int			Resume(uint8* pBuffer, int nTotalSize);
		// 将一个数据附加在缓冲区后面
		int			PushPacket(const uint8* pCode, int nLength);
		// 从缓冲区取出一个消息
		int			GetHeadPacket(uint8* pCode, int &rLength);
		// 从缓冲区取出一个消息
		int			PeekHeadPacket(uint8* pCode, int& rLength);
		// 从缓冲区删除一个消息
		int			DeleteHeadPacket();
		// 从指定偏移处取出一个消息
		int			GetOnePacket(int nOffset, int nLength, uint8* pOutCode, int& rOutLength);
		// 转储到文件
		int			DumpToFile(const char* pFileName);
		// 从文件里加载
		int			LoadFromFile(const char* pFileName);
		// 清空队列
		void		ClearBuffer();
		// 是否为空
		bool		IsEmpty();
		// 得到剩余空间
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