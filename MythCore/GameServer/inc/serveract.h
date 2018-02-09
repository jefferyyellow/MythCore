#ifndef __SERVERACTIVITY_H__
#define __SERVERACTIVITY_H__
#include "commontype.h"
// 开服活动
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
	/// 大类型
	byte		mType;
	// 子类型
	byte		mSubType;
	// 类型内部的ID
	short		mID;
	/// 开始时间
	int			mStartTime;	
	/// 结束时间
	int			mEndTime;
};

/// 通用活动
class CGeneralActivity
{

};
#endif