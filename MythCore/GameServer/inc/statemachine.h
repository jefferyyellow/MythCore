#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__
template<class ClassType, int Size>
class CStateMachine
{
public:
	typedef int (ClassType::*StateFunc)();
public:
	CStateMachine()
	{
		mCurState = 0;
		mpClass = NULL;
	}
	~CStateMachine(){}
	void init(ClassType* pClass, int nCurState)
	{
		mpClass = pClass;
		mCurState = nCurState;
	}
private:
	class CState
	{
	public:
		CState()
		{
			mStateFunc = NULL;
			mLeftTime = 0;
			mMaxTime = 0;
		}
		~CState(){}
		StateFunc	mStateFunc;		// ״̬����
		int			mLeftTime;		// ʣ��ʱ��(����)
		int			mMaxTime;		// ����ʱ��(����)
	};
	
public:
	int		getCurState() const { return mCurState; }
	void	setCurState(int nValue) { mCurState = nValue; }

	void	addState(int nState, int nMaxTime, StateFunc pFunc)
	{
		if (nState < 0 || nState >= Size)
		{
			return;
		}
		mStateData[nState].mStateFunc = pFunc;
		mStateData[nState].mMaxTime = nMaxTime;
		mStateData[nState].mLeftTime = nMaxTime;
	}

	void	checkState()
	{
		if (NULL == mStateData[mCurState].mStateFunc)
		{
			return;
		}

		int nNewState = (mpClass->*mStateData[mCurState].mStateFunc)();
		if (nNewState < 0)
		{
			return;
		}

		switchState(nNewState);
	}

	void	switchState(int nState)
	{
		if (nState < 0 || nState >= Size)
		{
			return;
		}
		// ����Ѿ�������״̬�ˣ�ֱ�ӷ��ذ�
		if (mCurState == nState)
		{
			return;
		}
		// ����Ϊ��ǰʱ��
		mCurState  = nState;
		// ����ʱ��
		mStateData[nState].mLeftTime = mStateData[nState].mMaxTime;
	}
	
	bool elapse(unsigned int nTickOffset)
	{
		// ����ʱ��
		mStateData[mCurState].mLeftTime -= nTickOffset;
		if (mStateData[mCurState].mLeftTime < 0)
		{
			return true;
		}
		return false;
	}


private:
	CState		mStateData[Size];		// ״̬������״̬
	int			mCurState;				// ��ǰ״̬
	ClassType*	mpClass;
};
#endif