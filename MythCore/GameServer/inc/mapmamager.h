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

	/// ʵ���ƶ�
	void		onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos);
	/// �õ���������
	void		getVisibleRect(CEntityCharacter* pEntity, CMythRect& rRect);
	/// ��ʵ������ͼ��Ԫ
	bool		addEntityToMapUnit(CEntity* pEntity);
	/// ��ʵ������ͼ��Ԫ����
	void		onAddEntityToMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�
	bool		removeEntityFromMapUnit(CEntity* pEntity);
	/// ��ʵ��ӵ�ͼ��Ԫ���Ƴ�����
	void		onRemoveEntityFromMapUnit(CEntityCharacter* pEntity, CMythRect& rSrcRect, CMythRect& rDesRect);
	/// �ڵ�ͼ�д���ʵ�崥��
	void		onCreateEntityToMapUnit(CEntityCharacter* pEntity);
	/// �ڵ�ͼ���Ƴ�ʵ�崥��
	void		onRemoveEntityToMapUnit(CEntityCharacter* pEntity);


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
	/// ֪ͨ����������ٸ�NPC
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
	/// ��ͼ����
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
	/// ������ͼ
	int					createMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, short nLength, short nWidth);
	/// ͨ�������õ���ͼ
	CMap*				getMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex);
	/// ����ͼ�����ͼ�б���
	bool				insertMap(unsigned short nLineID, unsigned short nMapID, int nMapIndex, CMap* pMap);

private:
	MAP_LIST			mMapList;
};
#endif