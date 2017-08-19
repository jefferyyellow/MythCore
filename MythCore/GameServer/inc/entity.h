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

	// ��ǰѪֵ
	uint32		getCurHP() const { return mCurHP; }
	void		setCurHP(uint32 nValue){ mCurHP = nValue; }

	// ��ǰħֵ
	uint32		getCurMP() const { return mCurMP; }
	void		setCurMP(uint32 nValue) { mCurMP = nValue; }

private:
	/// ���Ѫֵ
	uint32		mMaxHP;
	/// ���ħֵ
	uint32		mMaxMP;
	/// ������
	uint32		mAttack;
	/// ������
	uint32		mDefence;

	/// ��ǰѪֵ
	uint32		mCurHP;
	/// ��ǰħֵ
	uint32		mCurMP;
};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}


public:
	/// ˢ�����Ѫ
	virtual		int RefreshMaxHP()			= 0;
	/// ˢ�����ħ
	virtual		int RefreshMaxMP()			= 0;
	/// ˢ�¹�����
	virtual		int RefreshAttack()			= 0;
	/// ˢ�·�����
	virtual		int RefreshDefence()		= 0;

	/// ˢ�����Ѫ��ս�����ԣ�
	virtual		int RefreshMaxHPFight()		= 0;
	/// ˢ�����ħ��ս�����ԣ�
	virtual		int RefreshMaxMPFight()		= 0;
	/// ˢ�¹�������ս�����ԣ�
	virtual		int RefreshAttackFight()	= 0;
	/// ˢ�·�������ս�����ԣ�
	virtual		int RefreshDefenceFight()	= 0;

public:
	uint32		getTempID() const { return mTempID; }
	void		setTempID(uint32 nValue) { mTempID = nValue; }

private:
	/// ģ��ID
	uint32			mTempID;
	/// �������ԣ��Ƚ��ȶ������ԣ�
	CFightProperty	mBaseProperty;
	/// ս�����ԣ������������ԣ������ݱ����ԣ�
	CFightProperty  mFightProperty;
};

#endif