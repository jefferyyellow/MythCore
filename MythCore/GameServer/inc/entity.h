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

	uint32		getMaxMP() const { return mMaxMP; }
	void		setMaxMP(uint32 nValue){ mMaxMP = nValue; }

	uint32		getAttack() const { return mAttack; }
	void		setAttack(uint32 nValue) { mAttack = nValue; }

	uint32		setDefence() const { return mDefence; }
	void		getDefence(uint32 nValue) { mDefence = nValue; }

	// 当前血值
	uint32		getCurHP() const { return mCurHP; }
	void		setCurHP(uint32 nValue){ mCurHP = nValue; }

	// 当前魔值
	uint32		getCurMP() const { return mCurMP; }
	void		setCurMP(uint32 nValue) { mCurMP = nValue; }

private:
	/// 最大血值
	uint32		mMaxHP;
	/// 最大魔值
	uint32		mMaxMP;
	/// 攻击力
	uint32		mAttack;
	/// 防御力
	uint32		mDefence;

	/// 当前血值
	uint32		mCurHP;
	/// 当前魔值
	uint32		mCurMP;
};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}


public:
	/// 刷新最大血
	virtual		int RefreshMaxHP()			= 0;
	/// 刷新最大魔
	virtual		int RefreshMaxMP()			= 0;
	/// 刷新攻击力
	virtual		int RefreshAttack()			= 0;
	/// 刷新防御力
	virtual		int RefreshDefence()		= 0;

	/// 刷新最大血（战斗属性）
	virtual		int RefreshMaxHPFight()		= 0;
	/// 刷新最大魔（战斗属性）
	virtual		int RefreshMaxMPFight()		= 0;
	/// 刷新攻击力（战斗属性）
	virtual		int RefreshAttackFight()	= 0;
	/// 刷新防御力（战斗属性）
	virtual		int RefreshDefenceFight()	= 0;

public:
	uint32		getTempID() const { return mTempID; }
	void		setTempID(uint32 nValue) { mTempID = nValue; }

private:
	/// 模板ID
	uint32			mTempID;
	/// 基础属性（比较稳定的属性）
	CFightProperty	mBaseProperty;
	/// 战斗属性（包括基础属性，包括容变属性）
	CFightProperty  mFightProperty;
};

#endif