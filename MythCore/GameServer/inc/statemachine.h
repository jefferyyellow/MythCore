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
		StateFunc	mStateFunc;		// 状态函数
		int			mLeftTime;		// 剩余时间(毫秒)
		int			mMaxTime;		// 最大的时间(毫秒)
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
			if (NULL != mSwitchFailure)
			{
				(mpClass->*mSwitchFailure)();
			}
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
	
	bool elapse(unsigned int nTickOffset)
	{
		// 重置时间
		mStateData[mCurState].mLeftTime -= nTickOffset;
		if (mStateData[mCurState].mLeftTime < 0)
		{
			return true;
		}
		return false;
	}

	void setTime(unsigned int nTime)
	{
		mStateData[mCurState].mLeftTime = nTime;
	}

	void setSwitchFailure(StateFunc pFailureFunc)
	{
		mSwitchFailure = pFailureFunc;
	}

private:
	CState		mStateData[Size];		// 状态机所有状态
	int			mCurState;				// 当前状态
	ClassType*	mpClass;				// 关联的类指针
	StateFunc	mSwitchFailure;			// 切换失败的回调
};
#endif