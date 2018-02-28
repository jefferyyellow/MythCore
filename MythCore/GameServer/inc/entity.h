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
class CEntityPlayer;
class CEntityCreator;
class CEntity : public CObj
{
public:
	typedef CBlockMemory<CShareList<int>::CShareListNode<int>, 20000, 1000>	PLAYER_ALLOC;
	typedef CShareList<int> PLAYER_LIST;
public:
	CEntity(){}
	virtual ~CEntity(){}
	
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

	PLAYER_LIST&	getVisiblePlayer() { return mVisiblePlayer; }


public:
	static CEntity* createEntity(EmEntityType eType);
	static void		destroyEntity(CEntity* pEntity);
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	int			getTempID() const { return mTempID; }
	void		setTempID(int nValue) { mTempID = nValue; }

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

	bool			isPlayer(){return emEntityType_Player == mEntityType;}
	bool			isItem(){return emEntityType_Item == mEntityType;}
protected:
	/// ģ��ID
	int						mTempID;
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
	virtual ~CEntityCharacter(){}

public:
	/// ˢ��ս������
	virtual void	refreshFightProperty(){}
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	// ��ǰѪֵ
	int			getCurHP() const { return mCurHP; }
	void		setCurHP(int nValue){ mCurHP = nValue; }

	// ��ǰħֵ
	int			getCurMP() const { return mCurMP; }
	void		setCurMP(int nValue) { mCurMP = nValue; }

public:
	void		reduceHp(CEntityCharacter* pSrcEntity, int nNum);
	void		addHp(CEntityCharacter* pSrcEntity, int nNum);
	/// ʵ����������
	virtual void	onDie(CEntityCharacter* pKiller);
protected:
	/// ս������
	int					mFightProperty[emPropertyTypeMax];
	/// ��ǰѪֵ
	int					mCurHP;
	/// ��ǰħֵ
	int					mCurMP;
};

/// NPC��
class CEntityNPC : public CEntityCharacter
{
public:
	CEntityNPC(){}
	virtual ~CEntityNPC(){}
	
public:
	/// ˢ��ս������
	virtual void	refreshFightProperty(){}
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator)=0;

public:
	/// ���л�������Ϣ��PB��
	void			serializeSceneInfoToPB(PBNpcSceneInfo* pbNpcInfo);

};

/// Ogre��
class CEntityOgre : public CEntityNPC
{
public:
	CEntityOgre(){}
	virtual ~CEntityOgre(){}

public:
	/// ˢ��ս������
	virtual void	refreshFightProperty();
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator);
	/// ʵ����������
	virtual void	onDead(CEntityCharacter* pKiller);
	/// ��������
	void			deadDrop(CEntityPlayer* pPlayer, CTplOgre* pTplOgre);
};

/// ����NPC
class CEntityFuncNPC : public CEntityNPC
{
public:
	CEntityFuncNPC(){}
	~CEntityFuncNPC(){}

public:
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator);
};

/// ����ĵ�����
class CEntityItem : public CEntityNPC
{
public:
	CEntityItem()
	{
	}
	virtual ~CEntityItem()
	{
	}

public:
	short getItemNum()const{return mItemNum;}
	void setItemNum(short val){mItemNum = val;}
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator);

private:
	/// ������Ŀ
	short				mItemNum;
};
#endif