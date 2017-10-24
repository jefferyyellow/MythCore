#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#include "blockmemory.h"
#include "sharelist.h"
#include "geometrytype.h"
#include "entitytype.h"
#include "template.h"
using namespace Myth;
class PBNpcSceneInfo;
#define PLAYER_NAME_LENGTH   32
class CEntity : public CObj
{
public:
	typedef CBlockMemory<CShareList<uint32>::CShareListNode<uint32>, 20000, 1000>	PLAYER_ALLOC;
	typedef CShareList<uint32> PLAYER_LIST;
public:
	CEntity(){}
	~CEntity(){}
	
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

	PLAYER_LIST&	getVisiblePlayer() { return mVisiblePlayer; }


public:
	static CEntity* createEntity(EmEntityType eType);

public:
	unsigned short	getMapID() const { return mMapID; }
	void			setMapID(unsigned short nValue) { mMapID = nValue; }

	unsigned short	getLineID() const { return mLineID; }
	void			setLineID(unsigned short nValue) { mLineID = nValue; }

	int				getMapIndex() const { return mMapIndex; }
	void			setMapIndex(int nValue) { mMapIndex = nValue; }

	void			setPos(int nX, int nY)
	{
		mPos.mX = nX;
		mPos.mY = nY;
	}

	void			setPos(CMythPoint& rPos){	mPos = rPos;}
	CMythPoint&		getPos(){return mPos;}
	int				getPosX(){return mPos.mX;}
	int				getPosY(){return mPos.mY;}

	EmEntityType	getEntityType() const { return mEntityType; }
	void			setEntityType(EmEntityType nValue) { mEntityType = nValue; }

protected:
	/// �������
	PLAYER_LIST				mVisiblePlayer;
	/// �ɼ���ҷ�����(����ÿ��ȥ��ͼ����������������ҹ㲥�������൱��һ�����棩
	static	PLAYER_ALLOC	mVisiblePlayerAlloc;
	/// ��ID
	unsigned short			mLineID;
	/// ��ͼID
	unsigned short			mMapID;
	/// ��ͼ����
	int						mMapIndex;
	/// ��ͼ�е�λ��
	CMythPoint				mPos;
	/// ʵ������
	EmEntityType			mEntityType;
};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter(){}
	~CEntityCharacter(){}

public:


public:
	uint32		getTempID() const { return mTempID; }
	void		setTempID(uint32 nValue) { mTempID = nValue; }
	/// ˢ��ս������
	virtual void	RefreshFightProperty(){}

	// ��ǰѪֵ
	uint32		getCurHP() const { return mCurHP; }
	void		setCurHP(uint32 nValue){ mCurHP = nValue; }

	// ��ǰħֵ
	uint32		getCurMP() const { return mCurMP; }
	void		setCurMP(uint32 nValue) { mCurMP = nValue; }
protected:
	/// ģ��ID
	uint32					mTempID;
	/// ս������
	int						mFightProperty[emPropertyTypeMax];
	/// ��ǰѪֵ
	uint32					mCurHP;
	/// ��ǰħֵ
	uint32					mCurMP;
};

/// NPC��
class CEntityNPC : public CEntityCharacter
{
public:
	CEntityNPC(){}
	~CEntityNPC(){}
	
public:
	/// ���л�������Ϣ��PB��
	void			serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo);

};

/// Ogre��
class CEntityOgre : public CEntityNPC
{
public:
	CEntityOgre(){}
	~CEntityOgre(){}

public:
	/// ˢ��ս������
	virtual void	RefreshFightProperty();
};

/// ����NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC(){}
	~CEntityFuncNPC(){}
};

/// ����ĵ�����
class CEntityItem : public CEntityNPC
{
public:
	CEntityItem()
	{
	}
	~CEntityItem()
	{
	}

private:
	/// ������Ŀ
	short				mItemNum;
};
#endif