#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#include "blockmemory.h"
#include "sharelist.h"
#include "geometrytype.h"
#include "entitytype.h"
using namespace Myth;
class PBNpcSceneInfo;
#define PLAYER_NAME_LENGTH   32
class CEntity : public CObj
{
public:
	typedef CBlockMemory<CShareList<uint32>::CShareListNode<uint32>, 20000, 1000>	PLAYER_ALLOC;
	typedef CShareList<uint32> PLAYER_LIST;
public:
	CEntity(){}
	~CEntity(){}
	
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

	PLAYER_LIST&	getVisiblePlayer() { return mVisiblePlayer; }

public:
	unsigned short	getMapID() const { return mMapID; }
	void			setMapID(unsigned short nValue) { mMapID = nValue; }

	unsigned short	getLineID() const { return mLineID; }
	void			setLineID(unsigned short nValue) { mLineID = nValue; }

	int				getMapIndex() const { return mMapIndex; }
	void			setMapIndex(int nValue) { mMapIndex = nValue; }

	void			setPos(int nX, int nY)
	{
		mPos.mX = nX;
		mPos.mY = nY;
	}

	void			setPos(CMythPoint& rPos){	mPos = rPos;}
	CMythPoint&		getPos(){return mPos;}
	int				getPosX(){return mPos.mX;}
	int				getPosY(){return mPos.mY;}

	EmEntityType	getEntityType() const { return mEntityType; }
	void			setEntityType(EmEntityType nValue) { mEntityType = nValue; }

protected:
	/// �������
	PLAYER_LIST				mVisiblePlayer;
	/// �ɼ���ҷ�����(����ÿ��ȥ��ͼ����������������ҹ㲥�������൱��һ�����棩
	static	PLAYER_ALLOC	mVisiblePlayerAlloc;
	/// ��ID
	unsigned short			mLineID;
	/// ��ͼID
	unsigned short			mMapID;
	/// ��ͼ����
	int						mMapIndex;
	/// ��ͼ�е�λ��
	CMythPoint				mPos;
	/// ʵ������
	EmEntityType			mEntityType;
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

protected:
	/// ģ��ID
	uint32					mTempID;
	/// �������ԣ��Ƚ��ȶ������ԣ�
	CFightProperty			mBaseProperty;
	/// ս�����ԣ������������ԣ������ݱ����ԣ�����BUFF���ԣ�
	CFightProperty			mFightProperty;

};

/// NPC��
class CEntityNPC : public CEntityCharacter
{
public:
	CEntityNPC(){}
	~CEntityNPC(){}
	
public:
	/// ���л�������Ϣ��PB��
	void			serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo);

};

/// Ogre��
class CEntityOgre : public CEntityNPC
{
public:
	CEntityOgre(){}
	~CEntityOgre(){}
public:

};

/// ����NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC();
	~CEntityFuncNPC(){}
};

/// ����ĵ�����
class CEntityItem : public CEntityNPC
{
public:
	CEntityItem()
	{
	}
	~CEntityItem()
	{
	}

private:
	/// ������Ŀ
	short				mItemNum;
};
#endif