#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#define PLAYER_NAME_LENGTH   32
class CEntity : public CObj
{
public:
	CEntity(){}
	~CEntity(){}
	
protected:

};

class CFightProperty
{
public:
	uint32		getMaxHP() const { return mMaxHP; }
	void		setMaxHP(uint32 nValue) { mMaxHP = nValue; }

	uint32		getCurHP() const { return mCurHP; }
	void		setCurHP(uint32 nValue){ mCurHP = nValue; }

	uint32		getMaxMP() const { return mMaxMP; }
	void		setCurMP(uint32 nValue){ mCurMP = nValue; }

	uint32		getCurMP() const { return mCurMP; }
	void		setCurMP(uint32 nValue) { mCurMP = nValue; }

	uint32		getAttack() const { return mAttack; }
	void		setAttack(uint32 nValue) { mAttack = nValue; }

	uint32		setDefence() const { return mDefence; }
	void		getDefence(uint32 nValue) { mDefence = nValue; }

private:
	/// ���Ѫֵ
	uint32		mMaxHP;
	/// ��ǰѪֵ
	uint32		mCurHP;
	/// ���ħֵ
	uint32		mMaxMP;
	/// ��ǰħֵ
	uint32		mCurMP;
	/// ������
	uint32		mAttack;
	/// ������
	uint32		mDefence;
}

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}

public:
	uint32		getTempID() const { return mTempID; }
	void		setTempID(uint32 nValue) { mTempID = nValue; }

private:
	/// ģ��ID
	uint32			mTempID;
	CFightProperty	mBaseProperty;
	CFightProperty  mFightProperty;
};

#endif