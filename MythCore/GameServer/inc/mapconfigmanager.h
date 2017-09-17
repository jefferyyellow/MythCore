#ifndef __MAPCONFIGMANAGER_H__
#define __MAPCONFIGMANAGER_H__
#include "singleton.h"
#include "maptype.h"
#include "geometrytype.h"
#include <vector>
using namespace Myth;

class CMap;
// ������
class CMapPortalConfig
{
public:
	CMapPortalConfig()
	{}
	~CMapPortalConfig()
	{}

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
		mLength = 0;
		mWidth = 0;
		mMapType = emMapType_None;
		mMapUnitData = NULL;
	}
	~CMapConfig()
	{
	}


public:
	/// ���ص�ͼ����
	bool					loadMapConfig(unsigned short nMapID);
	/// ���ص�ͼNPC����
	bool					loadMapNPCConfig(unsigned short nMapID);
	/// �����ô�����ͼ
	int						createMapFromConfig(CMap* pMap);

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
	/// ��ͼ����
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
		memset(mMapConfig, 0, sizeof(mMapConfig));
	}
	~CMapConfigManager()
	{
	}

public:
	/// �����ô�����ͼ
	int						createMapFromConfig(CMap* pMap);
	/// �������е�ͼ
	bool					createAllMapFromConfig();
	/// ���ص�ͼ����
	bool					loadMapConfig(const char* pMapListFile);
	/// ͨ����ͼID�õ���ͼ����
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