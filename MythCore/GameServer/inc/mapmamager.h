#ifndef __MAPMAMAGER_H__
#define __MAPMAMAGER_H__
#include "singleton.h"
#include "sharelist.h"
#include "blockmemory.h"
#include "maptype.h"
#include "entityplayer.h"
#include <map>
#include <vector>

class CEntity;
class CEntityCharacter;
class CMythPoint;
class CMythRect;
using namespace Myth;
class CMapUnit
{
public:
	typedef CBlockMemory<CShareList<uint32>::CShareListNode<uint32>, 2000, 200> ENTITY_ALLOC;
	typedef CShareList<uint32> ENTITY_LIST;
public:
	CMapUnit()
	{
		mBlockData = 0;
	}

	~CMapUnit()
	{

	}

public:
	unsigned short		getBlockData() const { return mBlockData; }
	void				setBlockData(unsigned short nValue) { mBlockData = nValue; }

	void				pushEntity(uint32 nObjID);
	void				removeEntity(uint32 nObjID);

	ENTITY_LIST&		GetEntityList() { return mEntityList; }

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
		mLength = 0;
		mWidth = 0;
		mMapUnit = NULL;
	}
	~CMap()
	{
		clear();
	}

public:
	int				init(short nLength, short nWidth);
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
	void		onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos);
	/// 得到可以区域
	void		getVisibleRect(CEntityCharacter* pEntity, CMythRect& rRect);
	/// 把实体加入地图单元
	bool		addEntityToMapUnit(CEntity* pEntity);
	/// 把实体加入地图单元触发
	void		onAddEntityToMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 将实体从地图单元中移除
	bool		removeEntityFromMapUnit(CEntity* pEntity);
	/// 将实体从地图单元中移除触发
	void		onRemoveEntityFromMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// 在地图中创建实体触发
	void		onCreateEntityToMapUnit(CEntityCharacter* pEntity);
	/// 在地图中移除实体触发
	void		onRemoveEntityToMapUnit(CEntityCharacter* pEntity);


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
	/// 通知其他玩家销毁该NPC
	void		destroyNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList);

	
public:
	short			getLength() const { return mLength; }
	void			setLength(short nValue) { mLength = nValue; }

	short			getWidth() const { return mWidth; }
	void			setWidth(short nValue) { mWidth = nValue; }

	unsigned short	getMapID() const { return mMapID; }
	void			setMapID(unsigned short nValue) { mMapID = nValue; }

	unsigned short	getLineID() const { return mLineID; }
	void			setLineID(unsigned short nValue) { mLineID = nValue; }
	
	int				getMapIndex() const { return mMapIndex; }
	void			setMapIndex(int nValue) { mMapIndex = nValue; }

	EmMapType		getMapType() const { return mMapType; }
	void			setMapType(EmMapType nValue) { mMapType = nValue; }

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
	/// 地图类型
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
	}

	~CMapManager()
	{
	}

public:
	/// 创建地图
	int					createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// 通过参数得到地图
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// 将地图插入地图列表中
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);

private:
	MAP_LIST			mMapList;
};
#endif