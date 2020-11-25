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
class CMapCell
{
public:
	typedef CBlockMemory<CShareList<int>::CShareListNode<int>, 2000, 200> ENTITY_ALLOC;
	typedef CShareList<int> ENTITY_LIST;
public:
	CMapCell()
	{
	}

	~CMapCell()
	{
	}
	void				init()
	{
        mBlockData = 0;
	}

public:
	/// 加入实体
	void				pushEntity(int nObjID);
	/// 移除实体
	void				removeEntity(int nObjID);
	/// 当前地图格子是否有改实体
	bool				checkEntity(int nObjID);
	/// 得到实体数量
	int					getEntityNum();

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
	typedef std::vector<int>		ENTITY_LIST;
	typedef std::vector<int>		PLAYER_LIST;
public:
	CMap()
	{
		init();
	}
	~CMap()
	{
		clear();
	}

public:
	void			init()
	{
        mMapCell = NULL;
        mLength = 0;
        mWidth = 0;
        mLineID = 0;
        mMapID = 0;
        mMapIndex = 0;
        mMapType = emMapType_None;
        mNoPlayerTime = 0;
	}
	int				initSize(short nLength, short nWidth);
	void			clear();

	CMapCell*		getMapCell(short nPosX, short nPosY)
	{
		if (nPosX < mLength && nPosY < mWidth)
		{
			return mMapCell + (nPosY * mLength + nPosX);
		}
		return NULL;
	}

	/// 实体移动
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);
	/// 得到可以区域
	void		getVisibleRect(CEntity* pEntity, CMythRect& rRect);
	/// 把实体加入地图单元
	bool		addEntityToMapCell(CEntity* pEntity);
	/// 把实体加入地图单元触发
	void		onAddEntityToMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 将实体从地图单元中移除
	bool		removeEntityFromMapCell(CEntity* pEntity);
	/// 将实体从地图单元中移除触发
	void		onRemoveEntityFromMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 在地图中创建实体
	bool		createEntityToMap(CEntity* pEntity, CMythPoint& rPos);
	/// 在地图中创建实体触发
	void		onCreateEntityToMap(CEntity* pEntity);
	/// 在地图中移除实体
	bool		removeEntityFromMap(CEntity* pEntity);
	/// 在地图中移除实体触发
	void		onRemoveEntityFromMap(CEntity* pEntity);

	/// 创建玩家
	void		createPlayer(CEntityPlayer& rPlayer);
	/// 创建实体
	CEntity*	createEntity(CEntityCreator* pCreator);

	/// 销毁玩家
	void		destroyPlayer(CEntityPlayer& rPlayer);
	/// 销毁实体
	void		destroyEntity(CEntity* pEntity);
	/// 销毁地图上所有的实体（不包含玩家）
	void		destroyAllEntity();

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
    CMapCell* getMapCell(){ return mMapCell;}
    void setMapCell(CMapCell* value){ mMapCell = value;}

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

    ENTITY_LIST& getEntityList(){ return mEntityList;}

    PLAYER_LIST& getPlayerList(){ return mPlayerList;}

    time_t getNoPlayerTime(){ return mNoPlayerTime;}
    void setNoPlayerTime(time_t value){ mNoPlayerTime = value;}
	/// end autocode

private:
	/// 地图单元
	CMapCell*			mMapCell;
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
	/// 实体列表(除玩家以外)
	ENTITY_LIST			mEntityList;
	/// 玩家列表
	PLAYER_LIST			mPlayerList;
	/// 检测到没有玩家的设置的时间戳
	time_t				mNoPlayerTime;
};

#define  MAKE_MAP_KEY(LineID, MapID, MapIndex) ( (LineID << 48) | (MapID << 32) | MapIndex)
#define	 MAP_KEY_TO_LINE_ID(nKey) (nKey >> 48)
#define  MAP_KEY_TO_MAP_ID(nKey) (nKey >> 32 & 0x0000FFFF)
#define	 MAP_KEY_TO_MAP_INDEX(nKey) (nKey & 0xFFFFFFFF)
class CMapManager : public CSingleton<CMapManager>
{
	friend class CSingleton<CMapManager>;
public:
	typedef std::map<uint64, CMap*> MAP_LIST;
	typedef std::set<uint64> TIMER_DESTROY_SET;
private:
	CMapManager()
	{
		init();
	}

	~CMapManager()
	{
		clear();
	}
	void init()
	{
	}

	void				clear();

public:
	/// 创建地图
	CMap*				createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// 通过参数得到地图
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// 将地图插入地图列表中
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);
	/// 销毁地图
	void				destroyMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);

public:
	void				checkAutoDestoryMap();

public:
	/// autocode don't edit
	/// end autocode

	TIMER_DESTROY_SET&	getTimerDestroyList(){return mTimerDestroyList;}
	MAP_LIST&			getMapList(){return mMapList;}
private:
	/// 地图列表
	MAP_LIST			mMapList;
	/// 定时删除列表
	TIMER_DESTROY_SET	mTimerDestroyList;
};
#endif