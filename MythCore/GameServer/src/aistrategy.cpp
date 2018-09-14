#include "aistrategy.h"
#include "entity.h"
#include "timemanager.h"
void CAIStrategy::update(uint64 nTickCount)
{
	if (mOwner->getAliveState() != emAliveState_Alive)
	{
		return;
	}
	mStates[mCurState]->update(this);
}

/// �л�״̬
void CAIStrategy::changeState(EmAIState emState)
{
	mStates[mCurState]->exitState();
	mCurState = emState;
	mStates[mCurState]->enterState();
}

/// �л���վ��״̬
void CAIStrategy::changeStandState(int nTimer)
{
	mStandState.setEndTime(nTimer + CTimeManager::Inst()->getCurrTime());
	changeState(emAIState_Stand);
}

/// ���Ŀ��
void CAIStrategy::clearTarget()
{
	mTargetObjID = 0;
}