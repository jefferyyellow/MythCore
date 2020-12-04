#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__
#include "messagefactory.h"
#define MESSAGE_QUEUE_RESERVER_LEN 2	// Ԥ�����ֵĳ���
// ���������߳�֮���ͨ�ţ�����
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

	/// ѹ��һ����Ϣ
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
	/// ȡ��һ����Ϣ
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
	/// ȡ��һ����Ϣ
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
	/// ɾ��һ����Ϣ
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
	// ��ն���
	inline void			ClearBuffer()
	{
		mBegin = mEnd = 0;
	}
	// �Ƿ�Ϊ��
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
	// �õ�ʣ��ռ�
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