#ifndef __MAPMAMAGER_H__
#define __MAPMAMAGER_H__
#include "singleton.h"
#include "sharelist.h"
#include "blockmemory.h"
#include "maptype.h"
#include "entityplayer.h"
#include <map>

class CEntity;
class CEntityCharacter;
class CMythPoint;
class CMythRect;
class CEntityCreator;
using namespace Myth;
class CMapUnit
{
public:
	typedef CBlockMemory<CShareList<int>::CShareListNode<int>, 2000, 200> ENTITY_ALLOC;
	typedef CShareList<int> ENTITY_LIST;
public:
	CMapUnit()
	{
	}

	~CMapUnit()
	{
	}
	void				init()
	{
        mBlockData = 0;
	}

public:
	void				pushEntity(int nObjID);
	void				removeEntity(int nObjID);

	/// autocode don't edit!!!
    unsigned short getBlockData(){ return mBlockData;}
    void setBlockData(unsigned short value){ mBlockData = value;}

    ENTITY_LIST& getEntityList(){ return mEntityList;}
	/// end autocode

private:
	/// 阻挡数据
	unsigned short			mBlockData;
	/// 实体列表
	ENTITY_LIST				mEntityList;
	/// 实体列表分配器
	static ENTITY_ALLOC		mEntityAlloc;
};

class CMap
{
public:
	CMap()
	{
		
	}
	~CMap()
	{
		clear();
	}

public:
	void			init()
	{
        mMapUnit = NULL;
        mLength = 0;
        mWidth = 0;
        mLineID = 0;
        mMapID = 0;
        mMapIndex = 0;
        mMapType = emMapType_None;
	}

	int				initSize(short nLength, short nWidth);
	void			clear();

	CMapUnit*		getMapUnit(short nPosX, short nPosY)
	{
		if (nPosX < mLength && nPosY < mWidth)
		{
			return mMapUnit + (nPosY * mLength + nPosX);
		}
		return NULL;
	}

	/// 实体移动
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);
	/// 得到可以区域
	void		getVisibleRect(CEntity* pEntity, CMythRect& rRect);
	/// 把实体加入地图单元
	bool		addEntityToMapUnit(CEntity* pEntity);
	/// 把实体加入地图单元触发
	void		onAddEntityToMapUnit(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 将实体从地图单元中移除
	bool		removeEntityFromMapUnit(CEntity* pEntity);
	/// 将实体从地图单元中移除触发
	void		onRemoveEntityFromMapUnit(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 在地图中创建实体
	bool		createEntityToMap(CEntity* pEntity, CMythPoint& rPos);
	/// 在地图中创建实体触发
	void		onCreateEntityToMap(CEntity* pEntity);
	/// 在地图中移除实体
	bool		removeEntityFromMap(CEntity* pEntity);
	/// 在地图中移除实体触发
	void		onRemoveEntityFromMap(CEntity* pEntity);

	/// 创建玩家
	void		onCreatePlayer(CEntityPlayer* pPlayer);
	/// 创建实体
	CEntity*	createEntity(CEntityCreator* pCreator);

	/// 销毁玩家
	void		onDestroyPlayer(CEntityPlayer* pPlayer);
	/// 销毁实体
	void		DestroyEntity(CEntity* pEntity);

	/// 通知其他玩家创建该玩家
	void		createPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList);
	/// 通知该玩家创建其他玩家
	void		createPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer);
	/// 通知该玩家创建NPC列表
	void		createNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer);
	/// 通知该玩家创建NPC
	void		createNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList);
	/// 通知其他玩家销毁该玩家
	void		destroyPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList);
	/// 通知该玩家销毁其他玩家
	void		destroyPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer);
	/// 通知该玩家销毁NPC列表
	void		destroyNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer);	
	/// 通知其他玩家销毁该NPC
	void		destroyNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList);

	
public:
	/// autocode don't edit
    CMapUnit* getMapUnit(){ return mMapUnit;}
    void setMapUnit(CMapUnit* value){ mMapUnit = value;}

    short getLength(){ return mLength;}
    void setLength(short value){ mLength = value;}

    short getWidth(){ return mWidth;}
    void setWidth(short value){ mWidth = value;}

    unsigned short getLineID(){ return mLineID;}
    void setLineID(unsigned short value){ mLineID = value;}

    unsigned short getMapID(){ return mMapID;}
    void setMapID(unsigned short value){ mMapID = value;}

    int getMapIndex(){ return mMapIndex;}
    void setMapIndex(int value){ mMapIndex = value;}

    EmMapType getMapType(){ return mMapType;}
    void setMapType(EmMapType value){ mMapType = value;}
	/// end autocode

private:
	/// 地图单元
	CMapUnit*			mMapUnit;
	/// 长度
	short				mLength;
	/// 宽度
	short				mWidth;
	/// 线ID
	unsigned short		mLineID;
	/// 地图ID
	unsigned short		mMapID;
	/// 地图索引
	int					mMapIndex;
	/// 地图类型, default:emMapType_None
	EmMapType			mMapType;
};

#define  MAKE_MAP_KEY(LineID, MapID, MapIndex) ( (LineID << 48) | (MapID << 32) | MapIndex)
class CMapManager : public CSingleton<CMapManager>
{
	friend class CSingleton<CMapManager>;
public:
	typedef std::map<uint64, CMap*> MAP_LIST;
private:
	CMapManager()
	{
		init();
	}

	~CMapManager()
	{
	}
	void init()
	{
	}

public:
	/// 创建地图
	CMap*				createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// 通过参数得到地图
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// 将地图插入地图列表中
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);

public:
	/// autocode don't edit
	/// end autocode

private:
	MAP_LIST			mMapList;
};
#endif