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
		mTaskUnit(*this)
	{
		mName[0] = '\0';
		mRoleID = 0;
		mLastOffTime = 0;
		mOnTime = 0;
	}
	~CEntityPlayer(){}

public:
	/// ˢ�»�������
	void			RefreshBaseProperty();
	/// ˢ��ս������
	virtual void	RefreshFightProperty();

public:
	/// ���л�������Ϣ��PB��
	void			serializeSceneInfoToPB(PBPlayerSceneInfo* pbSceneInfo);

public:
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

	time_t			getLastSaveTime() const{return mLastSaveTime;}
	void			setLastSaveTime(time_t val)	{mLastSaveTime = val;}

	EmPlayerStatus getPlayerStauts(){ return mPlayerStauts;	}
	void setPlayerStauts(EmPlayerStatus val){ mPlayerStauts = val;}


	/// ���ֵ�Ԫ
	/// ���Ե�Ԫ
	CPropertyUnit&	GetPropertyUnit(){ return mPropertyUnit; }
	/// ���ߵ�Ԫ
	CItemUnit&		GetItemUnit(){ return mItemUnit; }
	/// ����Ԫ
	CTaskUnit&		GetTaskUnit(){ return mTaskUnit; }
	
private:
	/// socket������Ϣ
	CExchangeHead	mExhangeHead;
	/// ���Ե�Ԫ
	CPropertyUnit	mPropertyUnit;
	/// ���ߵ�Ԫ
	CItemUnit		mItemUnit;
	/// ����Ԫ
	CTaskUnit		mTaskUnit;

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
	/// ���״̬
	EmPlayerStatus	mPlayerStauts;
};
#endif 