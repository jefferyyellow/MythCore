#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#include "blockmemory.h"
#include "sharelist.h"
#include "geometrytype.h"
#include "entitytype.h"
#include "template.h"
using namespace Myth;
class PBNpcSceneInfo;
#define PLAYER_NAME_LENGTH   32
class CEntityPlayer;
class CEntity : public CObj
{
public:
	typedef CBlockMemory<CShareList<int>::CShareListNode<int>, 20000, 1000>	PLAYER_ALLOC;
	typedef CShareList<int> PLAYER_LIST;
public:
	CEntity(){}
	~CEntity(){}
	
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

	PLAYER_LIST&	getVisiblePlayer() { return mVisiblePlayer; }


public:
	static CEntity* createEntity(EmEntityType eType);

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

	bool			isPlayer(){return mEntityType == emEntityType_Player;}
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

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}

public:
	/// 刷新战斗属性
	virtual void	refreshFightProperty(){}

public:
	int			getTempID() const { return mTempID; }
	void		setTempID(int nValue) { mTempID = nValue; }
	// 当前血值
	int			getCurHP() const { return mCurHP; }
	void		setCurHP(int nValue){ mCurHP = nValue; }

	// 当前魔值
	int			getCurMP() const { return mCurMP; }
	void		setCurMP(int nValue) { mCurMP = nValue; }

public:
	void		reduceHp(CEntityCharacter* pSrcEntity, int nNum);
	void		addHp(CEntityCharacter* pSrcEntity, int nNum);
	/// 实体死亡处理
	virtual void	onDie(CEntityCharacter* pKiller);
protected:
	/// 模板ID
	int					mTempID;
	/// 战斗属性
	int					mFightProperty[emPropertyTypeMax];
	/// 当前血值
	int					mCurHP;
	/// 当前魔值
	int					mCurMP;
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
	/// 刷新战斗属性
	virtual void	refreshFightProperty();
	/// 实体死亡处理
	virtual void	onDead(CEntityCharacter* pKiller);
	/// 死亡掉落
	void			deadDrop(CEntityPlayer* pPlayer, CTplOgre* pTplOgre);
};

/// 功能NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC(){}
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

public:
	short getItemNum()const{return mItemNum;}
	void setItemNum(short val){mItemNum = val;}

private:
	/// 道具数目
	short				mItemNum;
};
#endif