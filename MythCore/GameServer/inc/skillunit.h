#ifndef __SKILLUNIT_H__
#define __SKILLUNIT_H__
#include "servercommon.h"
#include "skilltype.h"
#include "array.h"
#include "playersubunit.h"
#include "locallogjob.h"
using namespace Myth;
class CPlayerSkill
{
public:
	CPlayerSkill()
	{
		mSkillCD = 0;
		mSkillId = 0;
		mSkillLevel = 0;
	}
	~CPlayerSkill(){}
public:
	time_t getSkillCD()const{return mSkillCD;}
	void setSkillCD(time_t val){mSkillCD = val;}

	int getSkillId()const{return mSkillId;}
	void setSkillId(int val){mSkillId = val;}

	byte getSkillLevel()const{return mSkillLevel;}
	void setSkillLevel(byte val){mSkillLevel = val;}

private:
	/// 技能CD
	time_t	mSkillCD;
	/// 技能Id
	int		mSkillId;
	/// 技能等级
	byte	mSkillLevel;
};

class CSkillUnit : public CPlayerSubUnit
{
	typedef CArray<CPlayerSkill, MAX_PLAYER_SKILL_NUM> PLAYER_SKILL_LIST;
public:
	CSkillUnit(CEntityPlayer& rPlayer)
		: CPlayerSubUnit(rPlayer)
	{
		
	}
	
	~CSkillUnit()
	{
		
	}

	void				pushbackSkill(int nSkillID, int nSkillLevel)
	{
		if (mSkillList.size() >= mSkillList.capacity())
		{
			LOG_ERROR("Push back skill error, Skill list siz:%d capacity: %d", mSkillList.size(), mSkillList.capacity());
			return;
		}
		CPlayerSkill tPlayerSkill;
		tPlayerSkill.setSkillId(nSkillID);
		tPlayerSkill.setSkillLevel(nSkillLevel);
		mSkillList.push_back(tPlayerSkill);
	}

	CPlayerSkill*		getPlayerSkill(unsigned int nIndex)
	{
		if (nIndex >= mSkillList.size())
		{
			LOG_ERROR("Get player skill error, Skill list siz:%d capacity: %d", mSkillList.size(), mSkillList.capacity());
			return NULL;
		}

		return &mSkillList[nIndex];
	}

public:
	PLAYER_SKILL_LIST&	getSkillList(){return mSkillList;}

private:
	PLAYER_SKILL_LIST	mSkillList;
};
#endif