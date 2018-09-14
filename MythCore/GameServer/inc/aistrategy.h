#ifndef __AISTRATEGY_H__
#define __AISTRATEGY_H__
#include "aistate.h"
#include "obj.h"
class CEntity;
class CAIStrategy : public CObj
{
public:
	CAIStrategy()
	{
		mCurState = emAIState_Stand;
		memset(mStates, NULL, sizeof(mStates));
		init();
	}
	~CAIStrategy(){}
	void init()
	{
        mOwner = NULL;
        mTargetObjID = 0;
	}

public:
	void		update(uint64 nTickCount);
	/// 切换状态
	void		changeState(EmAIState emState);
	/// 切换到站立状态
	void		changeStandState(int nTimer);
	/// 清空目标
	void		clearTarget();
public:
	/// autocode don't edit !!!
    CEntity* getOwner(){ return mOwner;}
    void setOwner(CEntity* value){ mOwner = value;}

    int getTargetObjID(){ return mTargetObjID;}
    void setTargetObjID(int value){ mTargetObjID = value;}

    CAIStandState& getStandState(){ return mStandState;}

    CAIMoveState& getMoveState(){ return mMoveState;}

    CAIAttackState& getAttackState(){ return mAttackState;}
	/// end autocode
private:
	/// 策略主人
	CEntity*	mOwner;
	/// 目标的Obj ID
	int			mTargetObjID;

private:
	/// 站立状态
	CAIStandState	mStandState;
	/// 移动状态
	CAIMoveState	mMoveState;
	/// 攻击状态
	CAIAttackState	mAttackState;
	/// default:ignore
	EmAIState		mCurState;
	/// default:ignore
	CAIState*		mStates[emAIStateMax];
};
#endif