#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "obj.h"
#include "logintype.h"
#include "blockmemory.h"
#include "sharelist.h"
#include "geometrytype.h"
#include "entitytype.h"
#include "template.h"
#include "entitytimer.h"
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
	CEntity()
	{
		init();
	}
	virtual ~CEntity(){}
	
	void			init();
public:
	void			addVisiblePlayer(CEntity* pEntity);
	void			removeVisiblePlayer(CEntity* pEntity);

public:
	static CEntity* createEntity(EmEntityType eType);
	static void		destroyEntity(CEntity* pEntity);
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	void			setPos(int nX, int nY)
	{
		mPos.mX = nX;
		mPos.mY = nY;
	}

	void			setPos(CMythPoint& rPos){	mPos = rPos;}
	int				getPosX(){return mPos.mX;}
	int				getPosY(){return mPos.mY;}

	bool			isPlayer(){return emEntityType_Player == mEntityType;}
	bool			isItem(){return emEntityType_Item == mEntityType;}

public:
	/// autocode, don't edit!!!
    int getTempID(){ return mTempID;}
    void setTempID(int value){ mTempID = value;}

    PLAYER_LIST& getVisiblePlayer(){ return mVisiblePlayer;}

    unsigned short getLineID(){ return mLineID;}
    void setLineID(unsigned short value){ mLineID = value;}

    unsigned short getMapID(){ return mMapID;}
    void setMapID(unsigned short value){ mMapID = value;}

    int getMapIndex(){ return mMapIndex;}
    void setMapIndex(int value){ mMapIndex = value;}

    CMythPoint& getPos(){ return mPos;}

    EmEntityType getEntityType(){ return mEntityType;}
    void setEntityType(EmEntityType value){ mEntityType = value;}
	/// end autocode
protected:
	/// ģ��ID
	int						mTempID;
	/// �������
	PLAYER_LIST				mVisiblePlayer;
	/// �ɼ���ҷ�����(����ÿ��ȥ��ͼ����������������ҹ㲥�������൱��һ�����棩(ignore)
	static	PLAYER_ALLOC	mVisiblePlayerAlloc;
	/// ��ID
	unsigned short			mLineID;
	/// ��ͼID
	unsigned short			mMapID;
	/// ��ͼ����
	int						mMapIndex;
	/// ��ͼ�е�λ��
	CMythPoint				mPos;
	/// ʵ������ default:emEntityType_None
	EmEntityType			mEntityType;
};

class CEntityCharacter : public CEntity
{
public:
	CEntityCharacter()
	{
		init();
	}
	virtual ~CEntityCharacter(){}

	void		init();
public:
	/// ˢ��ս������
	virtual void	refreshFightProperty(){}
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator) = 0;

public:
	void		reduceHp(CEntityCharacter* pSrcEntity, int nNum);
	void		addHp(CEntityCharacter* pSrcEntity, int nNum);
	/// ʵ����������
	virtual void	onDie(CEntityCharacter* pKiller);

public:
	/// autocode don't edit!!!
    int getFightProperty(int nIndex)
    {
        if(nIndex < 0 || nIndex >= emPropertyTypeMax)
        {
            return 0;
        }
        return mFightProperty[nIndex];
    }
    void setFightProperty(int nIndex, int value)
    {
        if(nIndex < 0 || nIndex >= emPropertyTypeMax)
        {
            return;
        }
        mFightProperty[nIndex] = value;
    }

    int getCurHP(){ return mCurHP;}
    void setCurHP(int value){ mCurHP = value;}

    int getCurMP(){ return mCurMP;}
    void setCurMP(int value){ mCurMP = value;}
	/// end autocode
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
	/// ���ú��뼶��ʱ��(���ܳ���24��)
	int				setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);

private:
	///	��ʱ���б�
	CTimerList		mTimerList;
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

	void	init();

public:
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator);

public:
	/// autocode don't edit!!!
    short getItemNum(){ return mItemNum;}
    void setItemNum(short value){ mItemNum = value;}
	/// end autocode
private:
	/// ������Ŀ
	short				mItemNum;
};
#endif