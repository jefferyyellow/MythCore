#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#include "blockmemory.h"
#include "sharelist.h"
#include "geometrytype.h"
#include "entitytype.h"
#include "template.h"
#include "entitytimer.h"
using namespace Myth;
class PBNpcSceneInfo;
class CEntityPlayer;
class CEntityCreator;
class CEntity : public CObj
{
public:
	typedef CBlockMemory<CShareList<int>::CShareListNode<int>, 20000, 1000>	PLAYER_ALLOC;
	typedef CShareList<int> PLAYER_LIST;
public:
	CEntity()
	{
		init();
	}
	virtual ~CEntity(){}
	
	void			init();
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

public:
	static CEntity* createEntity(EmEntityType eType);
	static void		destroyEntity(CEntity* pEntity);
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	void			setPos(int nX, int nY)
	{
		mPos.mX = nX;
		mPos.mY = nY;
	}

	void			setPos(CMythPoint& rPos){	mPos = rPos;}
	int				getPosX(){return mPos.mX;}
	int				getPosY(){return mPos.mY;}

	bool			isPlayer(){return emEntityType_Player == mEntityType;}
	bool			isItem(){return emEntityType_Item == mEntityType;}

public:
	/// autocode, don't edit!!!
    int getTempID(){ return mTempID;}
    void setTempID(int value){ mTempID = value;}

    PLAYER_LIST& getVisiblePlayer(){ return mVisiblePlayer;}

    unsigned short getLineID(){ return mLineID;}
    void setLineID(unsigned short value){ mLineID = value;}

    unsigned short getMapID(){ return mMapID;}
    void setMapID(unsigned short value){ mMapID = value;}

    int getMapIndex(){ return mMapIndex;}
    void setMapIndex(int value){ mMapIndex = value;}

    CMythPoint& getPos(){ return mPos;}

    EmEntityType getEntityType(){ return mEntityType;}
    void setEntityType(EmEntityType value){ mEntityType = value;}
	/// end autocode
protected:
	/// 模板ID
	int						mTempID;
	/// 看见玩家
	PLAYER_LIST				mVisiblePlayer;
	/// 可见玩家分配器(避免每次去地图里搜索，尤其是玩家广播，这里相当于一个缓存）(ignore)
	static	PLAYER_ALLOC	mVisiblePlayerAlloc;
	/// 线ID
	unsigned short			mLineID;
	/// 地图ID
	unsigned short			mMapID;
	/// 地图索引
	int						mMapIndex;
	/// 地图中的位置
	CMythPoint				mPos;
	/// 实体类型 default:emEntityType_None
	EmEntityType			mEntityType;
};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter()
	{
		init();
	}
	virtual ~CEntityCharacter(){}

	void		init();
public:
	/// 刷新战斗属性
	virtual void	refreshFightProperty(){}
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	void		reduceHp(CEntityCharacter* pSrcEntity, int nNum);
	void		addHp(CEntityCharacter* pSrcEntity, int nNum);
	/// 实体死亡处理
	virtual void	onDie(CEntityCharacter* pKiller);

public:
	/// autocode don't edit!!!
    int getFightProperty(int nIndex)
    {
        if(nIndex < 0 || nIndex >= emPropertyTypeMax)
        {
            return 0;
        }
        return mFightProperty[nIndex];
    }
    void setFightProperty(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= emPropertyTypeMax)
        {
            return;
        }
        mFightProperty[nIndex] = value;
    }

    int getCurHP(){ return mCurHP;}
    void setCurHP(int value){ mCurHP = value;}

    int getCurMP(){ return mCurMP;}
    void setCurMP(int value){ mCurMP = value;}
	/// end autocode
protected:
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
	virtual ~CEntityNPC(){}
	
public:
	/// 刷新战斗属性
	virtual void	refreshFightProperty(){}
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator)=0;

public:
	/// 序列化场景信息到PB·
	void			serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo);

};

/// Ogre类
class CEntityOgre : public CEntityNPC
{
public:
	CEntityOgre(){}
	virtual ~CEntityOgre(){}

public:
	/// 刷新战斗属性
	virtual void	refreshFightProperty();
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator);
	/// 实体死亡处理
	virtual void	onDead(CEntityCharacter* pKiller);
	/// 死亡掉落
	void			deadDrop(CEntityPlayer* pPlayer, CTplOgre* pTplOgre);
	/// 设置毫秒级计时器(不能超过24天)
	int				setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);

private:
	///	计时器列表
	CTimerList		mTimerList;
};

/// 功能NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC(){}
	~CEntityFuncNPC(){}

public:
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator);
};

/// 掉落的道具类
class CEntityItem : public CEntityNPC
{
public:
	CEntityItem()
	{
		
	}
	virtual ~CEntityItem()
	{
	}

	void	init();

public:
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator);

public:
	/// autocode don't edit!!!
    short getItemNum(){ return mItemNum;}
    void setItemNum(short value){ mItemNum = value;}
	/// end autocode
private:
	/// 道具数目
	short				mItemNum;
};
#endif