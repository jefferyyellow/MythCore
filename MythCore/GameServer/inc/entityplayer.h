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
	///	基础属性
	int				mValue;
	///	基础属性脏标记,玩家上线的时候属性就是脏的，需要刷新
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
	/// 刷新基本属性
	void			refreshBaseProperty();
	/// 刷新战斗属性
	virtual void	refreshFightProperty();
	void			refreshFightProperty(int nPropertyType);
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator){};
	//// 得到玩家属性请求
	void			onGetPlayerPropertyRequest(Message* pMessage);

public:
	/// 序列化场景信息到PB
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);
	/// 设置毫秒级计时器(不能超过24天)
	int				setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);

public:
	/// 设置属性
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

	/// 设置属性脏标记
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
		// 取得基本属性
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
	// 用mTempID保存职业属性
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
	/// socket连接信息
	CExchangeHead	mExchangeHead;
	/// 属性单元
	CPropertyUnit	mPropertyUnit;
	/// 道具单元
	CItemUnit		mItemUnit;
	/// 任务单元
	CTaskUnit		mTaskUnit;
	/// 技能单元
	CSkillUnit		mSkillUnit;
	/// 开服活动单元
	CServerActivityUnit	mServerActUnit;
	/// VIP单元
	CVIPUnit		mVIPUnit;
	/// 时间单元
	CTimeUnit		mTimeUnit;

	/// 角色名字
	char			mName[PLAYER_NAME_LENGTH];
	/// 角色ID
	unsigned int	mRoleID;
	/// 基础属性
	CBaseProperty	mBaseProperty[emProTypeMax];
	/// 玩家状态 EmPlayerStatus
	byte			mPlayerStauts;
	/// 玩家存储状态 EmPlayerSaveStatus
	byte			mSaveStatus;
	/// 玩家加载状态 EmPlayerLoadStatus
	byte			mLoadStatus;
};
#endif 