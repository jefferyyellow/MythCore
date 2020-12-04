#include "aistate.h"
#include "aistrategy.h"
#include "objpool.h"
#include "entity.h"
#include "mapmodule.h"
#include "template.h"
using namespace Myth;
/// 进入状态调用
void CAIStandState::enterState()
{
	
}

/// 退出状态调用
void CAIStandState::exitState()
{

}

/// 更新状态调用
void CAIStandState::update(CAIStrategy* pStrategy)
{
	uint64 tNowTickCount = 0;//CSceneJob::Inst()->getLastTimerTick();
	if (tNowTickCount < mEndTime)
	{
		return;
	}
	if (pStrategy->getTargetObjID() > 0)
	{
		pStrategy->changeState(emAIState_Attack);
	}
	else
	{
		pStrategy->changeState(emAIState_Move);
	}
}

/// 进入状态调用
void CAIMoveState::enterState()
{
	mNextPos = CMythPoint(-1,-1);
}

/// 退出状态调用
void CAIMoveState::exitState()
{

}

/// 更新状态调用
void CAIMoveState::update(CAIStrategy* pStrategy)
{
	// 这一步还没走完，直接返回
	uint64 tNowTickCount = 0;//CSceneJob::Inst()->getLastTimerTick();
	if (tNowTickCount < mMoveEndTime)
	{
		return;
	}

	CEntity* pOwerEntity = pStrategy->getOwner();
	// 实体找到下一个点
	CMapModule::Inst()->onEntityMove(pStrategy->getOwner(), mNextPos);

	// 已经找到头了
	if (mPathList.size() <= 0)
	{
		pStrategy->changeStandState(2000);
		return;
	}

	int nTargetObjId = pStrategy->getTargetObjID();
	CEntity* pTargetEntity = NULL;
	if (nTargetObjId > 0)
	{
		pTargetEntity =static_cast<CEntity*>(CObjPool::Inst()->getObj(nTargetObjId));
		// 失去目标了
		if (NULL == pTargetEntity)
		{
			return;
		}
		// 检查是否能是否技能
		int nSkillID = pStrategy->getAttackState().canAttack(pOwerEntity->getPos(), pTargetEntity->getPos());
		if (nSkillID > 0)
		{
			return;
		}

		// 如果已经换了地方，需要重新寻路
		if (pTargetEntity->getPos() != mPathList[0])
		{
			// 检查是否技能能打到，如果打不到，重新寻路
		}

		if (mPathList.size() <= 0)
		{
			return;
		}
	}
	CMythPoint tNextTargetPos = mPathList[mPathList.size() - 1];
	if (mNextPos == tNextTargetPos)
	{
		// 已经到了下一个目标点，去掉该点
		mPathList.pop_back();
		// 达到目的地
		if (mPathList.size() <= 0)
		{
			return;
		}
		tNextTargetPos = mPathList[mPathList.size() - 1];
	}

	// 按照以前写的
	if (mNextPos.mX == tNextTargetPos.mX)
	{
		mNextPos.mY = tNextTargetPos.mY;
	}
	else if (mNextPos.mY == tNextTargetPos.mY)
	{
		if (tNextTargetPos.mX > mNextPos.mX)
		{
			mNextPos.mX += 1;
		}
		else
		{
			mNextPos.mX -= 1;
		}
	}
	else
	{
		mNextPos.mX = tNextTargetPos.mX;
		mNextPos.mY = tNextTargetPos.mY;
	}

	// 计算走这么远需要的时间
	int nTime = 300;
	mMoveEndTime += tNowTickCount + nTime;
}


/// 进入状态调用
void CAIAttackState::enterState()
{

}

/// 退出状态调用
void CAIAttackState::exitState()
{

}

/// 更新状态调用
void CAIAttackState::update(CAIStrategy* pStrategy)
{
	uint64 tNowTickCount = 0;//CSceneJob::Inst()->getLastTimerTick();
	CEntity* pOwner = pStrategy->getOwner();
	
	// 技能释放中
	if (tNowTickCount < mSkillTime)
	{
		return;
	}

	int nTargetObjID = pStrategy->getTargetObjID();
	CEntity* pTargetEntity = NULL;
	if (nTargetObjID > 0)
	{
		pTargetEntity = static_cast<CEntity*>(CObjPool::Inst()->getObj(nTargetObjID));
	}
	// 目标消失
	if (NULL == pTargetEntity)
	{
		// 返回出生点
		pStrategy->clearTarget();
		return;
	}

	// 没有选好的技能就选择一个技能
	if (mSkillID <= 0)
	{
		mSkillID = canAttack(pOwner->getPos(), pTargetEntity->getPos());
		// 没有可以用的技能
		if (mSkillID <= 0)
		{
			pStrategy->changeStandState(1000);
			return;
		}

		mSkillState = emSkillState_None;
	}

	CTplSkill* pTplSkill = static_cast<CTplSkill*>(CStaticData::searchTpl(mSkillID));
	if (NULL == pTplSkill)
	{
		return;
	}

	switch (mSkillState)
	{
		case emSkillState_None:
		{
			setSkillCD(mSkillID);
			mSkillTime = tNowTickCount + pTplSkill->mPrepareTime;
			mSkillState = emSkillState_Prepare;
			break;
		}
		case emSkillState_Prepare:
		{
			// 使用技能
			mSkillTime = tNowTickCount + pTplSkill->mActionTime;
			mSkillState = emSkillState_Use;
			useSkill();
			break;
		}
		case emSkillState_Use:
		{
			mSkillState = emSkillState_None;
			break;
		}
	}
}

int CAIAttackState::canAttack(CMythPoint& rPos, CMythPoint& rTargetPos)
{
	uint64 tNowTick = 0;//CSceneJob::Inst()->getLastTimerTick();
	for (size_t i = 0; i < mSkillList.size(); ++ i)
	{
		if (tNowTick < mSkillList[i].mCoolTime)
		{
			continue;
		}

		CTplSkill* pTplSkill = static_cast<CTplSkill*>(CStaticData::searchTpl(mSkillList[i].mID));
		if (NULL == pTplSkill)
		{
			continue;
		}

		// 检查技能范围
		return pTplSkill->mTempID;
	}
	return 0;
}


void CAIAttackState::setSkillCD(int nSkillID)
{
	uint64 tNowTick = 0;//CSceneJob::Inst()->getLastTimerTick();
	for (size_t i = 0; i < mSkillList.size(); ++i)
	{
		if (mSkillList[i].mID != nSkillID)
		{
			continue;
		}

		mSkillList[i].mCoolTime = tNowTick + mSkillList[i].mMaxCDTime;
		break;
	}
}

CAIAttackState::CSkillData* CAIAttackState::getSkillData(int nSkillID)
{
	for (size_t i = 0; i < mSkillList.size(); ++i)
	{
		if (mSkillList[i].mID != nSkillID)
		{
			continue;
		}
		return &mSkillList[i];
	}

	return NULL;
}

void CAIAttackState::useSkill()
{

}