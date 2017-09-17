#ifndef __MAPCONFIGMANAGER_H__
#define __MAPCONFIGMANAGER_H__
#include "singleton.h"
#include "maptype.h"
#include "geometrytype.h"
#include <vector>
using namespace Myth;

class CMap;
// 传送门
class CMapPortalConfig
{
public:
	CMapPortalConfig()
	{}
	~CMapPortalConfig()
	{}

	/// 目标地图ID
	unsigned short		mTargetMapID;
	// 传送门位置
	CMythPoint			mPos;
};

// NPC数据
class CMapNPCConfig
{
public:
	CMapNPCConfig()
	{}
	~CMapNPCConfig()
	{}

	/// 模板ID
	int					mTempID;
	/// NPC初始位置
	CMythPoint			mPos;
};

// 
class CMapConfig
{
public:
	/// 地图中传送门的数据
	typedef std::vector <CMapPortalConfig>	MAP_PORTAL_VECTOR;
	/// 地图中NPC的数据
	typedef std::vector<CMapNPCConfig>		MAP_NPC_VECTOR;
	/// 地图中复活点的数据
	typedef std::vector<CMythPoint>			MAP_RELIEVE_POS;

public:
	CMapConfig()
	{
		mLength = 0;
		mWidth = 0;
		mMapType = emMapType_None;
		mMapUnitData = NULL;
	}
	~CMapConfig()
	{
	}


public:
	/// 加载地图配置
	bool					loadMapConfig(unsigned short nMapID);
	/// 加载地图NPC配置
	bool					loadMapNPCConfig(unsigned short nMapID);
	/// 从配置创建地图
	int						createMapFromConfig(CMap* pMap);

public:
	short					getLength() const { return mLength; }
	void					setLength(short nValue) { mLength = nValue; }

	short					getWidth() const { return mWidth; }
	void					setWidth(short nValue) { mWidth = nValue; }

	EmMapType				getMapType() const { return mMapType; }
	void					setMapType(EmMapType eValue) { mMapType = eValue; }

private:
	/// 长度
	short				mLength;
	/// 宽度
	short				mWidth;
	/// 地图类型
	EmMapType			mMapType;
	///	地图格子数据
	unsigned short*		mMapUnitData;
	/// 地图中的传送门，前端自己创建就行，不用服务器创建实体
	MAP_PORTAL_VECTOR	mMapPortal;
	/// 地图中复活点的数据
	MAP_RELIEVE_POS		mRelievePos;
	/// 地图中的NPC
	MAP_NPC_VECTOR		mMapNPC;
};

class CMapConfigManager : public CSingleton < CMapConfigManager >
{
	friend class CSingleton< CMapConfigManager >;

private:
	CMapConfigManager()
	{
		memset(mMapConfig, 0, sizeof(mMapConfig));
	}
	~CMapConfigManager()
	{
	}

public:
	/// 从配置创建地图
	int						createMapFromConfig(CMap* pMap);
	/// 创建所有地图
	bool					createAllMapFromConfig();
	/// 加载地图配置
	bool					loadMapConfig(const char* pMapListFile);
	/// 通过地图ID得到地图配置
	CMapConfig*			getMapConfig(unsigned short nMapID)
	{
		if (nMapID > 0 && nMapID < MAX_MAP_ID)
		{
			return mMapConfig[nMapID];
		}
		return NULL;
	}

private:
	CMapConfig*			mMapConfig[MAX_MAP_ID];
};
#endif