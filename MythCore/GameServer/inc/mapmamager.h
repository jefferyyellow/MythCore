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
	/// �赲����
	unsigned short			mBlockData;
	/// ʵ���б�
	ENTITY_LIST				mEntityList;
	/// ʵ���б������
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

	/// ʵ���ƶ�
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);
	/// �õ���������
	void		getVisibleRect(CEntity* pEntity, CMythRect& rRect);
	/// ��ʵ������ͼ��Ԫ
	bool		addEntityToMapUnit(CEntity* pEntity);
	/// ��ʵ������ͼ��Ԫ����
	void		onAddEntityToMapUnit(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�
	bool		removeEntityFromMapUnit(CEntity* pEntity);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�����
	void		onRemoveEntityFromMapUnit(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// �ڵ�ͼ�д���ʵ��
	bool		createEntityToMap(CEntity* pEntity, CMythPoint& rPos);
	/// �ڵ�ͼ�д���ʵ�崥��
	void		onCreateEntityToMap(CEntity* pEntity);
	/// �ڵ�ͼ���Ƴ�ʵ��
	bool		removeEntityFromMap(CEntity* pEntity);
	/// �ڵ�ͼ���Ƴ�ʵ�崥��
	void		onRemoveEntityFromMap(CEntity* pEntity);

	/// �������
	void		onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	CEntity*	createEntity(CEntityCreator* pCreator);

	/// �������
	void		onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	void		DestroyEntity(CEntity* pEntity);

	/// ֪ͨ������Ҵ��������
	void		createPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList);
	/// ֪ͨ����Ҵ����������
	void		createPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer);
	/// ֪ͨ����Ҵ���NPC�б�
	void		createNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer);
	/// ֪ͨ����Ҵ���NPC
	void		createNPC2PlayerList(CEntityNPC* pNPC, std::vector<CEntityPlayer*>& rPlayerList);
	/// ֪ͨ����������ٸ����
	void		destroyPlayer2PlayerList(CEntityPlayer* pPlayer, std::vector<CEntityPlayer*>& rPlayerList);
	/// ֪ͨ����������������
	void		destroyPlayerList2Player(std::vector<CEntityPlayer*>& rPlayerList, CEntityPlayer* pPlayer);
	/// ֪ͨ���������NPC�б�
	void		destroyNPCList2Player(std::vector<CEntityNPC*>& rNPCList, CEntityPlayer* pPlayer);	
	/// ֪ͨ����������ٸ�NPC
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
	/// ��ͼ��Ԫ
	CMapUnit*			mMapUnit;
	/// ����
	short				mLength;
	/// ���
	short				mWidth;
	/// ��ID
	unsigned short		mLineID;
	/// ��ͼID
	unsigned short		mMapID;
	/// ��ͼ����
	int					mMapIndex;
	/// ��ͼ����, default:emMapType_None
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
	/// ������ͼ
	CMap*				createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// ͨ�������õ���ͼ
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// ����ͼ�����ͼ�б���
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);

public:
	/// autocode don't edit
	/// end autocode

private:
	MAP_LIST			mMapList;
};
#endif