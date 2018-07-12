#ifndef __MAPCONFIGMANAGER_H__
#define __MAPCONFIGMANAGER_H__
#include "singleton.h"
#include "maptype.h"
#include "geometrytype.h"
#include "servercommon.h"
using namespace Myth;

class CMap;
// ������
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

	/// Ŀ���ͼID
	unsigned short		mTargetMapID;
	// ������λ��
	CMythPoint			mPos;
};

// NPC����
class CMapNPCConfig
{
public:
	CMapNPCConfig()
	{}
	~CMapNPCConfig()
	{}

	void init()
	{
        mTempID = 0;
	}

	/// ģ��ID
	int					mTempID;
	/// NPC��ʼλ��
	CMythPoint			mPos;
};

// 
class CMapConfig
{
public:
	/// ��ͼ�д����ŵ�����
	typedef std::vector <CMapPortalConfig>	MAP_PORTAL_VECTOR;
	/// ��ͼ��NPC������
	typedef std::vector<CMapNPCConfig>		MAP_NPC_VECTOR;
	/// ��ͼ�и���������
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
        mMapUnitData = NULL;
	}

public:
	/// ���ص�ͼ����
	bool					loadMapConfig(unsigned short nMapID);
	/// ���ص�ͼNPC����
	bool					loadMapNPCConfig(unsigned short nMapID);
	/// �����ô�����ͼ
	int						createMapFromConfig(CMap* pMap);
	/// �����ŵ�λ��
	int						getPortalPos(unsigned short nMapID, CMythPoint& rPortalPos);
public:
	short					getLength() const { return mLength; }
	void					setLength(short nValue) { mLength = nValue; }

	short					getWidth() const { return mWidth; }
	void					setWidth(short nValue) { mWidth = nValue; }

	EmMapType				getMapType() const { return mMapType; }
	void					setMapType(EmMapType eValue) { mMapType = eValue; }

private:
	/// ����
	short				mLength;
	/// ���
	short				mWidth;
	/// ��ͼ���� default:emMapType_None
	EmMapType			mMapType;
	///	��ͼ��������
	unsigned short*		mMapUnitData;
	/// ��ͼ�еĴ����ţ�ǰ���Լ��������У����÷���������ʵ��
	MAP_PORTAL_VECTOR	mMapPortal;
	/// ��ͼ�и���������
	MAP_RELIEVE_POS		mRelievePos;
	/// ��ͼ�е�NPC
	MAP_NPC_VECTOR		mMapNPC;
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
	/// �����ô�����ͼ
	int						createMapFromConfig(CMap* pMap);
	/// �������е�ͼ
	bool					createAllMapFromConfig();
	/// ���ص�ͼ����
	bool					loadMapConfig(const char* pMapListFile);
	/// ͨ����ͼID�õ���ͼ����
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