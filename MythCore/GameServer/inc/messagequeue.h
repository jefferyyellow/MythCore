#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__
#include "messagefactory.h"
#define MESSAGE_QUEUE_RESERVER_LEN 2	// 预留部分的长度
// 用于两个线程之间的通信，无锁
template <int Capacity>
class CMessageQueue
{
public:
	CMessageQueue()
	{
		init();
	}
	~CMessageQueue()
	{

	}

	/// 压入一个消息
	int				pushMessage(Message* pMessage)
	{
		int nBegin = -1;
		int nEnd = -1;

		if (IsFull())
		{
			return -1;
		}

		GetCriticalData(nBegin, nEnd);
		if (nBegin < 0 || nBegin >= mSize || nEnd < 0 || nEnd >= mSize)
		{
			ClearBuffer();
			return -2;
		}
		mMsgList[nEnd] = pMessage;

		++ nEnd;
		nEnd = (nEnd + 1) % mSize;
		SetCriticalData(-1, nEnd);
	}
	/// 取出一个消息
	Message*	getHeadMessage()
	{
		if (IsEmpty())
		{
			return NULL;
		}
		int nBegin = -1;
		int nEnd = -1;
		GetCriticalData(nBegin, nEnd);
		Message* pMsg = mMsgList[nBegin];
		nBegin = (nBegin + 1) % mSize;
		SetCriticalData(nBegin, -1);
		return pMsg;
	}
	/// 取出一个消息
	int				peekHeadMessage()
	{
		if (IsEmpty())
		{
			return NULL;
		}
		int nBegin = -1;
		int nEnd = -1;
		GetCriticalData(nBegin, nEnd);
		Message* pMsg = mMsgList[nBegin];
		return pMsg;
	}
	/// 删除一个消息
	int				deleteHeadMessage()
	{
		if (IsEmpty())
		{
			return -1;
		}
		int nBegin = -1;
		int nEnd = -1;
		GetCriticalData(nBegin, nEnd);
		nBegin = (nBegin + 1) % mSize;
		SetCriticalData(nBegin, -1);
		return 0;
	}

	void				init()
	{
		mSize = Capacity;
		mBegin = 0;
		mEnd = 0;
		for (int i = 0; i < Capacity; ++i)
		{
			mMsgList[i] = NULL;
		}
	}
	// 清空队列
	inline void			ClearBuffer()
	{
		mBegin = mEnd = 0;
	}
	// 是否为空
	inline bool			IsEmpty()
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
	// 得到剩余空间
	int					GetLeftSpace()
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

		nMaxLength -= MESSAGE_QUEUE_RESERVER_LEN;
		return nMaxLength;

	}


	inline bool			IsFull()
	{
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
	Message*		mMsgList[Capacity];
};
#endif