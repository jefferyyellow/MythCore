#ifndef __MAPMAMAGER_H__
#define __MAPMAMAGER_H__
#include "singleton.h"
#include "sharelist.h"
#include "blockmemory.h"
#include "maptype.h"
#include <map>

using namespace Myth;
class CMapUnit
{
	typedef CBlockMemory<CShareList<uint32>::CShareListNode<uint32>, 200, 200> ENTITY_ALLOC;
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
	unsigned short getBlockData() const { return mBlockData; }
	void setBlockData(unsigned short nValue) { mBlockData = nValue; }

	void pushEntity(uint32 nObjID);
	void removeEntity(uint32 nObjID);

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
	}

public:
	bool			init(short nLength, short nWidth);

	CMapUnit*		getMapUnit(short nPosX, short nPosY)
	{
		if (nPosX < mLength && nPosY < mWidth)
		{
			return mMapUnit + (nPosY * mLength + nPosX);
		}
		return NULL;
	}

public:
	short			getLength() const { return mLength; }
	void			setLength(short nValue) { mLength = nValue; }

	short			getWidth() const { return mWidth; }
	void			setWidth(short nValue) { mWidth = nValue; }

	short			getMapID() const { return mMapID; }
	void			setMapID(short nValue) { mMapID = nValue; }

	short			getLineID() const { return mLineID; }
	void			setLineID(short nValue) { mLineID = nValue; }
	
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
	/// 地图ID
	short				mMapID;
	/// 线ID
	short				mLineID;
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

	CMap*				getMap(short nLineID, short nMapID, int nMapIndex);
	bool				insertMap(short nLineID, short nMapID, int nMapIndex, CMap* pMap);
private:
	MAP_LIST			mMapList;
};
#endif