#ifndef __AISTATE_H__
#define __AISTATE_H__
#include <vector>
#include "geometrytype.h"
#include "commontype.h"
#include "array.h"
using namespace Myth;
class CAIStrategy;
class CTplSkill;
enum EmAIState
{
	emAIState_Stand		=	0,		// վ��״̬
	emAIState_Move		=	1,		// �ƶ�״̬
	emAIState_Attack	=	2,		// ����״̬
	emAIStateMax
};

class CAIState
{
public:
	CAIState(){}
	~CAIState(){}

public:
	void		init()
	{
	}

public:
	/// ����״̬����
	virtual		void	enterState()	= 0;
	/// �˳�״̬����
	virtual		void	exitState()		= 0;
	/// ����״̬����
	virtual		void	update(CAIStrategy* pStrategy)		= 0;
};

/// վ��״̬
class CAIStandState : public CAIState
{
public:
	CAIStandState()
	{
		init();
	}
	~CAIStandState(){}

	void init()
	{
        mEndTime = 0;
	}
public:
	/// ����״̬����
	virtual		void	enterState();
	/// �˳�״̬����
	virtual		void	exitState();
	/// ����״̬����
	virtual		void	update(CAIStrategy* pStrategy);

public:
	/// autocode, don't edit!!!
    uint64 getEndTime(){ return mEndTime;}
    void setEndTime(uint64 value){ mEndTime = value;}
	/// end autocode
private:
	/// վ������ʱ��
	uint64			mEndTime;
};

#define MAX_PATH_POINT 20
/// �ƶ�״̬
class CAIMoveState : public CAIState
{
	typedef CArray<CMythPoint, MAX_PATH_POINT> PATH_LIST;
public:
	CAIMoveState()
	{
		init();
	}
	~CAIMoveState(){}

	void init()
	{
        mMoveEndTime = 0;
	}
public:
	/// ����״̬����
	virtual		void	enterState();
	/// �˳�״̬����
	virtual		void	exitState();
	/// ����״̬����
	virtual		void	update(CAIStrategy* pStrategy);

	/// autocode don't edit!!!
    PATH_LIST& getPathList(){ return mPathList;}

    CMythPoint& getNextPos(){ return mNextPos;}

    uint64 getMoveEndTime(){ return mMoveEndTime;}
    void setMoveEndTime(uint64 value){ mMoveEndTime = value;}
	/// end autocode
private:
	/// Ѱ··�����б�
	PATH_LIST	mPathList;
	/// ��һ��������
	CMythPoint	mNextPos;
	/// ÿһ����ʱ�����ʱ��
	uint64		mMoveEndTime;
};

#define MAX_AI_SKILL_NUM 8
/// ����״̬
class CAIAttackState
{
public:
	enum EmSkillState
	{
		emSkillState_None		= 0,		// ��ʼ״̬
		emSkillState_Prepare	= 1,		// ׼��״̬
		emSkillState_Use		= 2,		// ʹ��״̬
	};
	class CSkillData
	{
	public:
		uint64				mCoolTime;			// ÿ�����ܵ�cd
		int					mID;				// �����б�	
		int					mLevel;				// ���ܵȼ�
		int					mMaxCDTime;			// ��������CDʱ��
	};
	typedef CArray<CSkillData, MAX_AI_SKILL_NUM> SKILL_LIST;
public:
	CAIAttackState()
	{
		init();
	}
	~CAIAttackState(){}

	void init()
	{
        mSkillID = 0;
        mSkillTime = 0;
        mSkillState = emSkillState_None;
	}


public:
	/// ����״̬����
	virtual		void	enterState();
	/// �˳�״̬����
	virtual		void	exitState();
	/// ����״̬����
	virtual		void	update(CAIStrategy* pStrategy);

public:
	int					canAttack(CMythPoint& rPos, CMythPoint& rTargetPos);
	void				setSkillCD(int nSkillID);
	CSkillData*			getSkillData(int nSkillID);
	void				useSkill();
private:
	// ʵ��ļ����б�
	SKILL_LIST			mSkillList;
	// ��ǰ���ͷŵļ���ID
	int					mSkillID;
	/// ����ʱ��
	uint64				mSkillTime;
	/// ����״̬ default:emSkillState_None
	EmSkillState		mSkillState;
};
#endif