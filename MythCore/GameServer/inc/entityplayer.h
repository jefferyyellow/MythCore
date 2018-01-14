#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "propertyunit.h"
#include "itemunit.h"
#include "taskunit.h"
#include "skillunit.h"
#include "servercommon.h"
class PBPlayerSceneInfo;

class CBaseProperty
{
public:
	CBaseProperty()
	{
		mValue = 0;
		mDirty = false;
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
	///	基础属性脏标记
	bool			mDirty;
};

class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer()
		:mPropertyUnit(*this), 
		mItemUnit(*this), 
		mTaskUnit(*this),
		mSkillUnit(*this)
	{
		mName[0] = '\0';
		mRoleID = 0;
		mLastOffTime = 0;
		mOnTime = 0;
	}
	~CEntityPlayer(){}

public:
	/// 刷新基本属性
	void			refreshBaseProperty();
	/// 刷新战斗属性
	virtual void	refreshFightProperty();
	void			refreshFightProperty(int nPropertyType);
	/// 初始化
	virtual	void	initEntity(CEntityCreator* pCreator){};

public:
	/// 序列化场景信息到PB
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);

public:
	/// 设置属性
	int				getPropertyValue(EmPropertyType eType)
	{
		if (eType <= 0 || eType >= emPropertyTypeMax)
		{
			return 0;
		}

		return mBaseProperty[eType].getValue();
	}

	void			setPropertyValue(EmPropertyType eType, int nValue)
	{
		if (eType <= 0 || eType >= emPropertyTypeMax)
		{
			return;
		}

		mBaseProperty[eType].setValue(nValue);
	}

	/// 设置属性脏标记
	bool			getPropertyDirty(EmPropertyType eType)
	{
		if (eType <= 0 || eType >= emPropertyTypeMax)
		{
			return false;
		}

		return mBaseProperty[eType].getDirty();
	}

	void			setPropertyDirty(EmPropertyType eType, bool bDirty)
	{
		if (eType <= 0 || eType >= emPropertyTypeMax)
		{
			return;
		}
		mBaseProperty[eType].setDirty(bDirty);
	}

	/// 角色ID
	unsigned int	getRoleID(){ return mRoleID; }
	void			setRoleID(unsigned int nRoleID){ mRoleID = nRoleID; }
	/// 角色名字
	char*			getName(){ return mName; }
	void			setName(char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName)-1);
	}

	int				getLevel(){return mPropertyUnit.getLevel();}

	// 用mTempID保存职业属性
	int				getMetier(){return mTempID;}
	void			setMetier(int nMetier){mTempID = nMetier;}

	/// 与TCP交换用的头

	CExchangeHead&	GetExhangeHead(){ return mExhangeHead; }

	/// 上次的存储时间
	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	/// 玩家状态
	byte			getPlayerStauts(){ return mPlayerStauts;	}
	void			setPlayerStauts(byte val){ mPlayerStauts = val;}

	/// 存储状态
	byte			getSaveStatus()const{return mSaveStatus;}
	void			setSaveStatus(byte val){mSaveStatus = val;}
	void			setSaveStatusBit(byte val){mSaveStatus |= val;}

	/// 加载状态
	byte			getLoadStatus()const{return mLoadStatus;}
	void			setLoadStatus(byte val){mLoadStatus = val;}
	void			setLoadStatusBit(byte val){mLoadStatus |= val;}

	/// 各种单元
	/// 属性单元
	CPropertyUnit&	getPropertyUnit(){ return mPropertyUnit; }
	/// 道具单元
	CItemUnit&		getItemUnit(){ return mItemUnit; }
	/// 任务单元
	CTaskUnit&		getTaskUnit(){ return mTaskUnit; }
	/// 技能单元
	CSkillUnit&		getSkillUnit(){return mSkillUnit;}

private:
	/// socket连接信息
	CExchangeHead	mExhangeHead;
	/// 属性单元
	CPropertyUnit	mPropertyUnit;
	/// 道具单元
	CItemUnit		mItemUnit;
	/// 任务单元
	CTaskUnit		mTaskUnit;
	/// 技能单元
	CSkillUnit		mSkillUnit;

	/// 角色名字
	char			mName[PLAYER_NAME_LENGTH];
	/// 角色ID
	unsigned int	mRoleID;
	/// 上次下线时间
	time_t			mLastOffTime;
	/// 这次上线时间
	time_t			mOnTime;
	/// 基础属性
	CBaseProperty	mBaseProperty[emPropertyTypeMax];
	/// 上次存盘的时间
	time_t			mLastSaveTime;
	/// 玩家状态 EmPlayerStatus
	byte			mPlayerStauts;
	/// 玩家存储状态 EmPlayerSaveStatus
	byte			mSaveStatus;
	/// 玩家加载状态 EmPlayerLoadStatus
	byte			mLoadStatus;
};
#endif 