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
	/// �л�״̬
	void		changeState(EmAIState emState);
	/// �л���վ��״̬
	void		changeStandState(int nTimer);
	/// ���Ŀ��
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
	/// ��������
	CEntity*	mOwner;
	/// Ŀ���Obj ID
	int			mTargetObjID;

private:
	/// վ��״̬
	CAIStandState	mStandState;
	/// �ƶ�״̬
	CAIMoveState	mMoveState;
	/// ����״̬
	CAIAttackState	mAttackState;
	/// default:ignore
	EmAIState		mCurState;
	/// default:ignore
	CAIState*		mStates[emAIStateMax];
};
#endif