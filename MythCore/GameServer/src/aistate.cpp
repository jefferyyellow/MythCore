#include "aistate.h"
#include "aistrategy.h"
#include "objpool.h"
#include "entity.h"
#include "mapmodule.h"
#include "template.h"
using namespace Myth;
/// ����״̬����
void CAIStandState::enterState()
{
	
}

/// �˳�״̬����
void CAIStandState::exitState()
{

}

/// ����״̬����
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

/// ����״̬����
void CAIMoveState::enterState()
{
	mNextPos = CMythPoint(-1,-1);
}

/// �˳�״̬����
void CAIMoveState::exitState()
{

}

/// ����״̬����
void CAIMoveState::update(CAIStrategy* pStrategy)
{
	// ��һ����û���ֱ꣬�ӷ���
	uint64 tNowTickCount = 0;//CSceneJob::Inst()->getLastTimerTick();
	if (tNowTickCount < mMoveEndTime)
	{
		return;
	}

	CEntity* pOwerEntity = pStrategy->getOwner();
	// ʵ���ҵ���һ����
	CMapModule::Inst()->onEntityMove(pStrategy->getOwner(), mNextPos);

	// �Ѿ��ҵ�ͷ��
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
		// ʧȥĿ����
		if (NULL == pTargetEntity)
		{
			return;
		}
		// ����Ƿ����Ƿ���
		int nSkillID = pStrategy->getAttackState().canAttack(pOwerEntity->getPos(), pTargetEntity->getPos());
		if (nSkillID > 0)
		{
			return;
		}

		// ����Ѿ����˵ط�����Ҫ����Ѱ·
		if (pTargetEntity->getPos() != mPathList[0])
		{
			// ����Ƿ����ܴ򵽣�����򲻵�������Ѱ·
		}

		if (mPathList.size() <= 0)
		{
			return;
		}
	}
	CMythPoint tNextTargetPos = mPathList[mPathList.size() - 1];
	if (mNextPos == tNextTargetPos)
	{
		// �Ѿ�������һ��Ŀ��㣬ȥ���õ�
		mPathList.pop_back();
		// �ﵽĿ�ĵ�
		if (mPathList.size() <= 0)
		{
			return;
		}
		tNextTargetPos = mPathList[mPathList.size() - 1];
	}

	// ������ǰд��
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

	// ��������ôԶ��Ҫ��ʱ��
	int nTime = 300;
	mMoveEndTime += tNowTickCount + nTime;
}


/// ����״̬����
void CAIAttackState::enterState()
{

}

/// �˳�״̬����
void CAIAttackState::exitState()
{

}

/// ����״̬����
void CAIAttackState::update(CAIStrategy* pStrategy)
{
	uint64 tNowTickCount = 0;//CSceneJob::Inst()->getLastTimerTick();
	CEntity* pOwner = pStrategy->getOwner();
	
	// �����ͷ���
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
	// Ŀ����ʧ
	if (NULL == pTargetEntity)
	{
		// ���س�����
		pStrategy->clearTarget();
		return;
	}

	// û��ѡ�õļ��ܾ�ѡ��һ������
	if (mSkillID <= 0)
	{
		mSkillID = canAttack(pOwner->getPos(), pTargetEntity->getPos());
		// û�п����õļ���
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
			// ʹ�ü���
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

		// ��鼼�ܷ�Χ
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