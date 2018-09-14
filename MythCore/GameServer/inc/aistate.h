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
	emAIState_Stand		=	0,		// 站立状态
	emAIState_Move		=	1,		// 移动状态
	emAIState_Attack	=	2,		// 攻击状态
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
	/// 进入状态调用
	virtual		void	enterState()	= 0;
	/// 退出状态调用
	virtual		void	exitState()		= 0;
	/// 更新状态调用
	virtual		void	update(CAIStrategy* pStrategy)		= 0;
};

/// 站立状态
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
	/// 进入状态调用
	virtual		void	enterState();
	/// 退出状态调用
	virtual		void	exitState();
	/// 更新状态调用
	virtual		void	update(CAIStrategy* pStrategy);

public:
	/// autocode, don't edit!!!
    uint64 getEndTime(){ return mEndTime;}
    void setEndTime(uint64 value){ mEndTime = value;}
	/// end autocode
private:
	/// 站立结束时间
	uint64			mEndTime;
};

#define MAX_PATH_POINT 20
/// 移动状态
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
	/// 进入状态调用
	virtual		void	enterState();
	/// 退出状态调用
	virtual		void	exitState();
	/// 更新状态调用
	virtual		void	update(CAIStrategy* pStrategy);

	/// autocode don't edit!!!
    PATH_LIST& getPathList(){ return mPathList;}

    CMythPoint& getNextPos(){ return mNextPos;}

    uint64 getMoveEndTime(){ return mMoveEndTime;}
    void setMoveEndTime(uint64 value){ mMoveEndTime = value;}
	/// end autocode
private:
	/// 寻路路径点列表
	PATH_LIST	mPathList;
	/// 下一步的坐标
	CMythPoint	mNextPos;
	/// 每一步的时间结束时间
	uint64		mMoveEndTime;
};

#define MAX_AI_SKILL_NUM 8
/// 攻击状态
class CAIAttackState
{
public:
	enum EmSkillState
	{
		emSkillState_None		= 0,		// 开始状态
		emSkillState_Prepare	= 1,		// 准备状态
		emSkillState_Use		= 2,		// 使用状态
	};
	class CSkillData
	{
	public:
		uint64				mCoolTime;			// 每个技能的cd
		int					mID;				// 技能列表	
		int					mLevel;				// 技能等级
		int					mMaxCDTime;			// 技能最大的CD时间
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
	/// 进入状态调用
	virtual		void	enterState();
	/// 退出状态调用
	virtual		void	exitState();
	/// 更新状态调用
	virtual		void	update(CAIStrategy* pStrategy);

public:
	int					canAttack(CMythPoint& rPos, CMythPoint& rTargetPos);
	void				setSkillCD(int nSkillID);
	CSkillData*			getSkillData(int nSkillID);
	void				useSkill();
private:
	// 实体的技能列表
	SKILL_LIST			mSkillList;
	// 当前的释放的技能ID
	int					mSkillID;
	/// 技能时间
	uint64				mSkillTime;
	/// 技能状态 default:emSkillState_None
	EmSkillState		mSkillState;
};
#endif