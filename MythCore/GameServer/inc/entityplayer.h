#ifndef __ENTITYPLAYER_H__
#define __ENTITYPLAYER_H__
#include "entity.h"
#include "propertyunit.h"
#include "itemunit.h"
#include "taskunit.h"
class CEntityPlayer : public CEntityCharacter
{
public:
	CEntityPlayer()
		:mPropertyUnit(this), mItemUnit(this), mTaskUnit(this)
	{

	}
	~CEntityPlayer(){}

public:
	/// ��ɫID
	uint32			getRoleID(){ return mRoleID; }
	void			setRoleID(uint32 nRoleID){ mRoleID = nRoleID; }
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

	/// ��TCP�����õ�ͷ
	CExchangeHead	GetExhangeHead(){ return mExhangeHead; }
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
	uint32			mRoleID;
	/// �ϴ�����ʱ��
	time_t			mLastOffTime;
	/// �������ʱ��
	time_t			mOnTime;
};
#endif 