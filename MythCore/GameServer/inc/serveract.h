#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "commontype.h"
// �����
class CServerAct
{
public:
	CServerAct()
	{

	}
	~CServerAct(){}

	void		init()
	{
        mType = 0;
        mSubType = 0;
        mID = 0;
        mStartTime = 0;
        mEndTime = 0;
	}

public:
	// autocode
    byte getType(){ return mType;}
    void setType(byte value){ mType = value;}

    byte getSubType(){ return mSubType;}
    void setSubType(byte value){ mSubType = value;}

    short getID(){ return mID;}
    void setID(short value){ mID = value;}

    int getStartTime(){ return mStartTime;}
    void setStartTime(int value){ mStartTime = value;}

    int getEndTime(){ return mEndTime;}
    void setEndTime(int value){ mEndTime = value;}
	// end autocode

private:
	/// ������
	byte		mType;
	// ������
	byte		mSubType;
	// �����ڲ���ID
	short		mID;
	/// ��ʼʱ��
	int			mStartTime;	
	/// ����ʱ��
	int			mEndTime;
};

/// ͨ�û
class CGeneralActivity
{

};
#endif