#ifndef __MAIL_H__
#define __MAIL_H__
#include "gmlog.h"
#include "commontype.h"
#define MAX_GLOBAL_MAIL			32			// ����ȫ���ʼ�����
#define MAX_PLAYER_MAIL			40			// �����ʼ�����
#define MAX_MAIL_DURATION_TIME		(14 * SECONDS_PER_DAY) // 14�������

#define MAX_MAIL_TITLE		64			// �ʼ����ⳤ��
#define MAX_MAIL_BODY		300			// �ʼ����ݳ���
#define MAX_MAIL_ITEM		4			// �ʼ���������
#define MAX_GAME_EVENT_LOAD	500			// ÿ�μ��ص�GameEvent����

enum EmMailType
{
	emMailType_None			= 0,	// ��
	emMailType_Debug		= 1,	// Debug
	emMailType_GMGlobal		= 2,	// GMȫ���ʼ�
};


enum EmMailStatus
{
	emMailStatus_None		= 0,	// ��
	emMailStatus_Read		= 1,	// ����
	emMailStatus_Attatch	= 2,	// �����Ѿ���ȡ
};

// �ʼ�����
class CMailItem
{
public:
	CMailItem()
	{
		mItemID = 0;
		mItemNum = 0;
	}
public:
	int getItemID() const { return mItemID; }
	void setItemID(int nValue) { mItemID = nValue; }

	int getItemNum() const { return mItemNum; }
	void setItemNum(int nValue) { mItemNum = nValue; }

private:
	int			mItemID;
	int			mItemNum;
};

// �ʼ�
class CMail
{
public:
	CMail()
	{
		init();
	}

	void init()
	{
		mMailID = 0;
		mMailStatus = emMailStatus_None;
		mMailType = emMailType_None;
		mItemLog = emItemLog_None;
		mCreateTime = 0;
		mMailTitle[0] = '\0';
		mMailBody[0] = '\0';
	}

	void clearMailItem()
	{
		memset(mMailItem, 0, sizeof(mMailItem));
	}
	~CMail(){}


public:
	uint getMailID() const { return mMailID; }
	void setMailID(uint nValue) { mMailID = nValue; }

	EmMailStatus GetMailStatus() const { return mMailStatus; }
	void SetMailStatus(EmMailStatus nValue) { mMailStatus = nValue; }

	EmMailType getMailType() const { return mMailType; }
	void setMailType(EmMailType nValue) { mMailType = nValue; }

	EmItemLog getItemLog() const { return mItemLog; }
	void setItemLog(EmItemLog nValue) { mItemLog = nValue; }

	time_t getCreateTime() const { return mCreateTime; }
	void setCreateTime(time_t nValue) { mCreateTime = nValue; }

	char* getMailTitle(){return mMailTitle;}
	void setMailTitle(const char* pTitle)
	{
		if (NULL == pTitle)
		{
			return;
		}

		strncpy(mMailTitle, pTitle, sizeof(mMailTitle) - 1);
	}

	char* getMailBody(){return mMailBody;}
	void setMailBody(const char* pMailBody)
	{
		if (NULL == pMailBody)
		{
			return;
		}

		strncpy(mMailBody, pMailBody, sizeof(mMailBody) - 1);
	}

	CMailItem*	getMailItem(uint nIndex)
	{
		if (nIndex >= MAX_MAIL_ITEM)
		{
			return NULL;
		}

		return &mMailItem[nIndex];
	}

private:
	/// �ʼ�ID
	uint			mMailID;
	/// �ʼ�״̬
	EmMailStatus	mMailStatus;
	/// �ʼ�����
	EmMailType		mMailType;
	/// �ʼ�������־����
	EmItemLog		mItemLog;
	/// �ʼ�ʱ��
	time_t			mCreateTime;
	/// �ʼ�����
	char			mMailTitle[MAX_MAIL_TITLE];
	/// �ʼ�����
	char			mMailBody[MAX_MAIL_BODY];
	/// �ʼ�����
	CMailItem		mMailItem[MAX_MAIL_ITEM];
};


enum EmGameEventType
{
	emGameEvent_None			= 0,		// ������
};
#endif