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

/// ÇÐ»»×´Ì¬
void CAIStrategy::changeState(EmAIState emState)
{
	mStates[mCurState]->exitState();
	mCurState = emState;
	mStates[mCurState]->enterState();
}

/// ÇÐ»»µ½Õ¾Á¢×´Ì¬
void CAIStrategy::changeStandState(int nTimer)
{
	mStandState.setEndTime(nTimer + CTimeManager::Inst()->getCurrTime());
	changeState(emAIState_Stand);
}

/// Çå¿ÕÄ¿±ê
void CAIStrategy::clearTarget()
{
	mTargetObjID = 0;
}