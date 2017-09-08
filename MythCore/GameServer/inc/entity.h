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
	/// 看见玩家
	PLAYER_LIST				mVisiblePlayer;
	/// 可见玩家分配器(避免每次去地图里搜索，尤其是玩家广播，这里相当于一个缓存）
	static	PLAYER_ALLOC	mVisiblePlayerAlloc;
	/// 线ID
	unsigned short			mLineID;
	/// 地图ID
	unsigned short			mMapID;
	/// 地图索引
	int						mMapIndex;
	/// 地图中的位置
	CMythPoint				mPos;
	/// 实体类型
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

protected:
	/// 模板ID
	uint32					mTempID;
	/// 基础属性（比较稳定的属性）
	CFightProperty			mBaseProperty;
	/// 战斗属性（包括基础属性，包括容变属性，比如BUFF属性）
	CFightProperty			mFightProperty;

};

/// NPC类
class CEntityNPC : public CEntityCharacter
{
public:
	CEntityNPC(){}
	~CEntityNPC(){}
	
public:
	/// 序列化场景信息到PB·
	void			serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo);

};

/// Ogre类
class CEntityOgre : public CEntityNPC
{
public:
	CEntityOgre(){}
	~CEntityOgre(){}
public:

};

/// 功能NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC();
	~CEntityFuncNPC(){}
};

/// 掉落的道具类
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
	/// 道具数目
	short				mItemNum;
};
#endif