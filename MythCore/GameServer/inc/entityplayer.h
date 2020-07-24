#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "propertyunit.h"
#include "itemunit.h"
#include "taskunit.h"
#include "skillunit.h"
#include "vipunit.h"
#include "serveractivityunit.h"
#include "servercommon.h"
#include "timeunit.h"
class PBPlayerSceneInfo;

class CBaseProperty
{
public:
	CBaseProperty()
	{
		mValue = 0;
		mDirty = true;
	}
	virtual ~CBaseProperty()
	{

	}
public:
	int			getValue() const { return mValue; }
	void		setValue(int val) { mValue = val; }

	bool		getDirty() const { return mDirty; }
	void		setDirty(bool val) { mDirty = val; }

private:
	///	��������
	int				mValue;
	///	������������,������ߵ�ʱ�����Ծ�����ģ���Ҫˢ��
	bool			mDirty;
};

class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer()
		:mPropertyUnit(*this),
		mItemUnit(*this),
		mTaskUnit(*this),
		mSkillUnit(*this),
		mServerActUnit(*this),
		mVIPUnit(*this),
		mTimeUnit(*this)
	{
		init();
	}
	~CEntityPlayer(){}

	void			init()
	{
		mName[0] = '\0';
		mRoleID = 0;
		mPlayerStauts = 0;
		mSaveStatus = 0;
		mLoadStatus = 0;
	}
public:
	/// ˢ�»�������
	void			refreshBaseProperty();
	/// ˢ��ս������
	virtual void	refreshFightProperty();
	void			refreshFightProperty(int nPropertyType);
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator){};
	//// �õ������������
	void			onGetPlayerPropertyRequest(Message* pMessage);

public:
	/// ���л�������Ϣ��PB
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);
	/// ���ú��뼶��ʱ��(���ܳ���24��)
	int				setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);

public:
	/// ��������
	int				getPropertyValue(EmProType eType)
	{
		if (eType <= emProType_None || eType >= emProTypeMax)
		{
			return 0;
		}

		return mBaseProperty[eType].getValue();
	}

	void			setPropertyValue(EmProType eType, int nValue)
	{
		if (eType <= emProType_None || eType >= emProTypeMax)
		{
			return;
		}

		mBaseProperty[eType].setValue(nValue);
	}

	/// ������������
	bool			getPropertyDirty(EmProType eType)
	{
		if (eType <= emProType_None || eType >= emProTypeMax)
		{
			return false;
		}

		return mBaseProperty[eType].getDirty();
	}

	void			setPropertyDirty(EmProType eType, bool bDirty)
	{
		// ȡ�û�������
		eType = (EmProType)(eType < emProTypeMax ? eType : eType & ENTITY_PRO_MASK);
		if (eType <= emProType_None || eType >= emProTypeMax)
		{
			return;
		}
		mBaseProperty[eType].setDirty(bDirty);
	}

	void			setSaveStatusBit(byte val){ mSaveStatus |= val; }

	void			setLoadStatusBit(byte val){ mLoadStatus |= val; }
	byte			getLoadStatusBit(byte val){ return mLoadStatus & val; }
	// ��mTempID����ְҵ����
	int				getMetier(){ return mTempID; }
	void			setMetier(int nMetier){ mTempID = nMetier; }

	/// autocode don't edit!!!
	CExchangeHead& getExchangeHead(){ return mExchangeHead; }

	CPropertyUnit& getPropertyUnit(){ return mPropertyUnit; }

	CItemUnit& getItemUnit(){ return mItemUnit; }

	CTaskUnit& getTaskUnit(){ return mTaskUnit; }

	CSkillUnit& getSkillUnit(){ return mSkillUnit; }

	CServerActivityUnit& getServerActUnit(){ return mServerActUnit; }

	CTimeUnit& getTimeUnit(){return mTimeUnit;}
	CVIPUnit& getVIPUnit(){return mVIPUnit;}
    char* getName(){ return mName;}
    void setName(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mName, value, sizeof(mName) - 1);
    }

    unsigned int getRoleID(){ return mRoleID;}
    void setRoleID(unsigned int value){ mRoleID = value;}

    CBaseProperty* getBaseProperty(int nIndex)
    {
        if(nIndex < emProType_None || nIndex >= emProTypeMax)
        {
            return NULL;
        }
        return &mBaseProperty[nIndex];
    }
    void setBaseProperty(int nIndex, CBaseProperty& value)
    {
        if(nIndex < emProType_None || nIndex >= emProTypeMax)
        {
            return;
        }
        mBaseProperty[nIndex] = value;
    }

    byte getPlayerStauts(){ return mPlayerStauts;}
    void setPlayerStauts(byte value){ mPlayerStauts = value;}

    byte getSaveStatus(){ return mSaveStatus;}
    void setSaveStatus(byte value){ mSaveStatus = value;}

    byte getLoadStatus(){ return mLoadStatus;}
    void setLoadStatus(byte value){ mLoadStatus = value;}

	/// end autocode

	byte getLevel(){return mPropertyUnit.getLevel();}
	void setLevel(byte value){mPropertyUnit.setLevel(value);}
private:
	/// socket������Ϣ
	CExchangeHead	mExchangeHead;
	/// ���Ե�Ԫ
	CPropertyUnit	mPropertyUnit;
	/// ���ߵ�Ԫ
	CItemUnit		mItemUnit;
	/// ����Ԫ
	CTaskUnit		mTaskUnit;
	/// ���ܵ�Ԫ
	CSkillUnit		mSkillUnit;
	/// �������Ԫ
	CServerActivityUnit	mServerActUnit;
	/// VIP��Ԫ
	CVIPUnit		mVIPUnit;
	/// ʱ�䵥Ԫ
	CTimeUnit		mTimeUnit;

	/// ��ɫ����
	char			mName[PLAYER_NAME_LENGTH];
	/// ��ɫID
	unsigned int	mRoleID;
	/// ��������
	CBaseProperty	mBaseProperty[emProTypeMax];
	/// ���״̬ EmPlayerStatus
	byte			mPlayerStauts;
	/// ��Ҵ洢״̬ EmPlayerSaveStatus
	byte			mSaveStatus;
	/// ��Ҽ���״̬ EmPlayerLoadStatus
	byte			mLoadStatus;
};
#endif 