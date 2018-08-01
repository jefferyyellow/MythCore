#ifndef __MAPCONFIGMANAGER_H__
#define __MAPCONFIGMANAGER_H__
#include "singleton.h"
#include "maptype.h"
#include "geometrytype.h"
#include "servercommon.h"
using namespace Myth;

class CMap;
// 传送门
class CMapPortalConfig
{
public:
	CMapPortalConfig()
	{
		init();
	}
	~CMapPortalConfig()
	{}

	void	init()
	{
        mTargetMapID = 0;
	}

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
	{
		init();
	}
	~CMapNPCConfig()
	{}

	void init()
	{
        mTempID = 0;
	}

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
		init();
	}
	~CMapConfig()
	{
	}
	void init()
	{
        mLength = 0;
        mWidth = 0;
        mMapType = emMapType_None;
        mMapCellData = NULL;
        mAutoDestory = false;
        mAutoMirror = false;
	}

public:
	/// 加载地图配置
	bool					loadMapConfig(unsigned short nMapID);
	/// 加载地图NPC配置
	bool					loadMapNPCConfig(unsigned short nMapID);
	/// 从配置创建地图
	int						createMapFromConfig(CMap* pMap);
	/// 传送门的位置
	int						getPortalPos(unsigned short nMapID, CMythPoint& rPortalPos);
public:
	/// autocode don't edit
    short getLength(){ return mLength;}
    void setLength(short value){ mLength = value;}

    short getWidth(){ return mWidth;}
    void setWidth(short value){ mWidth = value;}

    EmMapType getMapType(){ return mMapType;}
    void setMapType(EmMapType value){ mMapType = value;}

    unsigned short* getMapCellData(){ return mMapCellData;}
    void setMapCellData(unsigned short* value){ mMapCellData = value;}

    MAP_PORTAL_VECTOR& getMapPortal(){ return mMapPortal;}

    MAP_RELIEVE_POS& getRelievePos(){ return mRelievePos;}

    MAP_NPC_VECTOR& getMapNPC(){ return mMapNPC;}

    bool getAutoDestory(){ return mAutoDestory;}
    void setAutoDestory(bool value){ mAutoDestory = value;}

    bool getAutoMirror(){ return mAutoMirror;}
    void setAutoMirror(bool value){ mAutoMirror = value;}
	/// end autocode
private:
	/// 长度
	short				mLength;
	/// 宽度
	short				mWidth;
	/// 地图类型 default:emMapType_None
	EmMapType			mMapType;
	///	地图格子数据
	unsigned short*		mMapCellData;
	/// 地图中的传送门，前端自己创建就行，不用服务器创建实体
	MAP_PORTAL_VECTOR	mMapPortal;
	/// 地图中复活点的数据
	MAP_RELIEVE_POS		mRelievePos;
	/// 地图中的NPC
	MAP_NPC_VECTOR		mMapNPC;
	/// 是否定时删除地图
	bool				mAutoDestory;
	/// 是否自动生成镜像
	bool				mAutoMirror;
};

class CMapConfigManager : public CSingleton < CMapConfigManager >
{
	friend class CSingleton< CMapConfigManager >;

private:
	CMapConfigManager()
	{
		init();
	}
	~CMapConfigManager()
	{
		clear();
	}

	void init()
	{
        memset(mMapConfig, NULL, sizeof(mMapConfig));
	}
	void clear();

public:
	/// 加载地图配置
	bool					loadMapConfig(const char* pMapListFile);
	/// 通过地图ID得到地图配置
	CMapConfig*				getMapConfig(unsigned short nMapID)
	{
		if (nMapID < MAX_MAP_ID)
		{
			return mMapConfig[nMapID];
		}
		return NULL;
	}
	int						getMapPortalPos(unsigned short nSrcMap, unsigned short nDesMap, CMythPoint& rPortalPos);

private:
	CMapConfig*			mMapConfig[MAX_MAP_ID];
};
#endif