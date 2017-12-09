#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "propertyunit.h"
#include "itemunit.h"
#include "taskunit.h"
class PBPlayerSceneInfo;

class CBaseProperty
{
public:
	CBaseProperty()
	{
		mValue = 0;
		mDirty = false;
	}
	~CBaseProperty()
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
		mTaskUnit(*this)
	{
		mName[0] = '\0';
		mRoleID = 0;
		mLastOffTime = 0;
		mOnTime = 0;
	}
	~CEntityPlayer(){}

public:
	/// 刷新基本属性
	void			RefreshBaseProperty();
	/// 刷新战斗属性
	virtual void	RefreshFightProperty();

public:
	/// 序列化场景信息到PB·
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);

public:
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

	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	EmPlayerStatus getPlayerStauts(){ return mPlayerStauts;	}
	void setPlayerStauts(EmPlayerStatus val){ mPlayerStauts = val;}


	/// 各种单元
	/// 属性单元
	CPropertyUnit&	GetPropertyUnit(){ return mPropertyUnit; }
	/// 道具单元
	CItemUnit&		GetItemUnit(){ return mItemUnit; }
	/// 任务单元
	CTaskUnit&		GetTaskUnit(){ return mTaskUnit; }
	
private:
	/// socket连接信息
	CExchangeHead	mExhangeHead;
	/// 属性单元
	CPropertyUnit	mPropertyUnit;
	/// 道具单元
	CItemUnit		mItemUnit;
	/// 任务单元
	CTaskUnit		mTaskUnit;

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
	/// 玩家状态
	EmPlayerStatus	mPlayerStauts;
};
#endif 