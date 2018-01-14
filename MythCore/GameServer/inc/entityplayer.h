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
	///	��������
	int				mValue;
	///	������������
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
	/// ˢ�»�������
	void			refreshBaseProperty();
	/// ˢ��ս������
	virtual void	refreshFightProperty();
	void			refreshFightProperty(int nPropertyType);
	/// ��ʼ��
	virtual	void	initEntity(CEntityCreator* pCreator){};

public:
	/// ���л�������Ϣ��PB
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);

public:
	/// ��������
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

	/// ������������
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

	/// ��ɫID
	unsigned int	getRoleID(){ return mRoleID; }
	void			setRoleID(unsigned int nRoleID){ mRoleID = nRoleID; }
	/// ��ɫ����
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

	// ��mTempID����ְҵ����
	int				getMetier(){return mTempID;}
	void			setMetier(int nMetier){mTempID = nMetier;}

	/// ��TCP�����õ�ͷ

	CExchangeHead&	GetExhangeHead(){ return mExhangeHead; }

	/// �ϴεĴ洢ʱ��
	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	/// ���״̬
	byte			getPlayerStauts(){ return mPlayerStauts;	}
	void			setPlayerStauts(byte val){ mPlayerStauts = val;}

	/// �洢״̬
	byte			getSaveStatus()const{return mSaveStatus;}
	void			setSaveStatus(byte val){mSaveStatus = val;}
	void			setSaveStatusBit(byte val){mSaveStatus |= val;}

	/// ����״̬
	byte			getLoadStatus()const{return mLoadStatus;}
	void			setLoadStatus(byte val){mLoadStatus = val;}
	void			setLoadStatusBit(byte val){mLoadStatus |= val;}

	/// ���ֵ�Ԫ
	/// ���Ե�Ԫ
	CPropertyUnit&	getPropertyUnit(){ return mPropertyUnit; }
	/// ���ߵ�Ԫ
	CItemUnit&		getItemUnit(){ return mItemUnit; }
	/// ����Ԫ
	CTaskUnit&		getTaskUnit(){ return mTaskUnit; }
	/// ���ܵ�Ԫ
	CSkillUnit&		getSkillUnit(){return mSkillUnit;}

private:
	/// socket������Ϣ
	CExchangeHead	mExhangeHead;
	/// ���Ե�Ԫ
	CPropertyUnit	mPropertyUnit;
	/// ���ߵ�Ԫ
	CItemUnit		mItemUnit;
	/// ����Ԫ
	CTaskUnit		mTaskUnit;
	/// ���ܵ�Ԫ
	CSkillUnit		mSkillUnit;

	/// ��ɫ����
	char			mName[PLAYER_NAME_LENGTH];
	/// ��ɫID
	unsigned int	mRoleID;
	/// �ϴ�����ʱ��
	time_t			mLastOffTime;
	/// �������ʱ��
	time_t			mOnTime;
	/// ��������
	CBaseProperty	mBaseProperty[emPropertyTypeMax];
	/// �ϴδ��̵�ʱ��
	time_t			mLastSaveTime;
	/// ���״̬ EmPlayerStatus
	byte			mPlayerStauts;
	/// ��Ҵ洢״̬ EmPlayerSaveStatus
	byte			mSaveStatus;
	/// ��Ҽ���״̬ EmPlayerLoadStatus
	byte			mLoadStatus;
};
#endif 