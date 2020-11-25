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
	/// ����ʵ��
	void				pushEntity(int nObjID);
	/// �Ƴ�ʵ��
	void				removeEntity(int nObjID);
	/// ��ǰ��ͼ�����Ƿ��и�ʵ��
	bool				checkEntity(int nObjID);
	/// �õ�ʵ������
	int					getEntityNum();

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

	/// ʵ���ƶ�
	void		onEntityMove(CEntity* pEntity, CMythPoint& rDesPos);
	/// �õ���������
	void		getVisibleRect(CEntity* pEntity, CMythRect& rRect);
	/// ��ʵ������ͼ��Ԫ
	bool		addEntityToMapCell(CEntity* pEntity);
	/// ��ʵ������ͼ��Ԫ����
	void		onAddEntityToMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�
	bool		removeEntityFromMapCell(CEntity* pEntity);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�����
	void		onRemoveEntityFromMapCell(CEntity* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// �ڵ�ͼ�д���ʵ��
	bool		createEntityToMap(CEntity* pEntity, CMythPoint& rPos);
	/// �ڵ�ͼ�д���ʵ�崥��
	void		onCreateEntityToMap(CEntity* pEntity);
	/// �ڵ�ͼ���Ƴ�ʵ��
	bool		removeEntityFromMap(CEntity* pEntity);
	/// �ڵ�ͼ���Ƴ�ʵ�崥��
	void		onRemoveEntityFromMap(CEntity* pEntity);

	/// �������
	void		createPlayer(CEntityPlayer& rPlayer);
	/// ����ʵ��
	CEntity*	createEntity(CEntityCreator* pCreator);

	/// �������
	void		destroyPlayer(CEntityPlayer& rPlayer);
	/// ����ʵ��
	void		destroyEntity(CEntity* pEntity);
	/// ���ٵ�ͼ�����е�ʵ�壨��������ң�
	void		destroyAllEntity();

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
	/// ��ͼ��Ԫ
	CMapCell*			mMapCell;
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
	/// ʵ���б�(���������)
	ENTITY_LIST			mEntityList;
	/// ����б�
	PLAYER_LIST			mPlayerList;
	/// ��⵽û����ҵ����õ�ʱ���
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
	/// ������ͼ
	CMap*				createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// ͨ�������õ���ͼ
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// ����ͼ�����ͼ�б���
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);
	/// ���ٵ�ͼ
	void				destroyMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);

public:
	void				checkAutoDestoryMap();

public:
	/// autocode don't edit
	/// end autocode

	TIMER_DESTROY_SET&	getTimerDestroyList(){return mTimerDestroyList;}
	MAP_LIST&			getMapList(){return mMapList;}
private:
	/// ��ͼ�б�
	MAP_LIST			mMapList;
	/// ��ʱɾ���б�
	TIMER_DESTROY_SET	mTimerDestroyList;
};
#endif