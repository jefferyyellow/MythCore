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
		StateFunc mStateFunc;		// 状态函数
		time_t mLeftTime;			// 剩余时间(毫秒)
		time_t mMaxTime;			// 最大的时间(毫秒)
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
		// 如果已经是这种状态了，直接返回吧
		if (mCurState == nState)
		{
			return;
		}
		// 设置为当前时间
		mCurState  = nState;
		// 重置时间
		mStateData[nState].mLeftTime = mStateData[nState].mMaxTime;
	}
	
private:
	CState	mStateData[Size];		// 状态机所有状态
	int		mCurState;				// 当前状态
};
#endif