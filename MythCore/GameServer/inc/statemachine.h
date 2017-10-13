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
	}
	~CStateMachine(){}
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
		StateFunc mStateFunc;		// ״̬����
		time_t mLeftTime;			// ʣ��ʱ��(����)
		time_t mMaxTime;			// ����ʱ��(����)
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

		int nNewState = (mStateData[mCurState].mStateFunc)();
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
	
private:
	CState	mStateData[Size];		// ״̬������״̬
	int		mCurState;				// ��ǰ״̬
};
#endif