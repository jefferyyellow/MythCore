#include "commontype.h"
#include "variable.h"
#define  MAX_LENGTH 10
#define  MAX_LENGTH2 15

class CPlusClassTest
{
public:
	typedef vector<CPlusClassTest*>		SUB_CLASS_VECTOR;
	typedef vector<CVariable*>		VARIABLE_VECTOR;

public:
	CPlusClassTest()
	{
		mOutClass = NULL;
	}
	~CPlusClassTest(){}

	void init()
	{
        mOutClass = NULL;
	}
	// autocode	DO NOT EDIT!
    SUB_CLASS_VECTOR* getSubClassList(int nIndex)
    {
        if(nIndex < 0 || nIndex >= MAX_LENGTH)
        {
            return NULL;
        }
        return &mSubClassList[nIndex];
    }
    void setSubClassList(int nIndex, SUB_CLASS_VECTOR& value)
    {
        if(nIndex < 0 || nIndex >= MAX_LENGTH)
        {
            return;
        }
        mSubClassList[nIndex] = value;
    }

    VARIABLE_VECTOR* getVariableList(int nIndex, int nIndex2)
    {
        if(nIndex < 0 || nIndex >= MAX_LENGTH)
        {
            return NULL;
        }
        if(nIndex2 < 0 || nIndex2 >= MAX_LENGTH2)
        {
            return NULL;
        }
        return &mVariableList[nIndex][nIndex2];
    }
    void setVariableList(int nIndex, int nIndex2, VARIABLE_VECTOR& value)
    {
        if(nIndex < 0 || nIndex >= MAX_LENGTH)
        {
            return;
        }
        if(nIndex2 < 0 || nIndex2 >= MAX_LENGTH2)
        {
            return;
        }
        mVariableList[nIndex][nIndex2] = value;
    }

    CPlusClassTest* getOutClass(){ return mOutClass;}
    void setOutClass(CPlusClassTest* value){ mOutClass = value;}
	// autocode
	class CNestClassTest
	{
	public:
		void init()
		{
            mPosZ = -1;
            memset(mA, 0, sizeof(mA));
            memset(mB, 0, sizeof(mB));
            mC[0] = '\0';
            memset(mD, 0, sizeof(mD));
            mPos = NULL;
		}
	public:
		// default: ignore
		/* int mPosX;*/
		// default: ignore
		int mPosY;
		// 位置 default: -1
		float mPosZ;
		int mA[MAX_LENGTH];
		int mB[MAX_LENGTH][MAX_LENGTH2];
		char mC[MAX_LENGTH];
		char mD[MAX_LENGTH][MAX_LENGTH2];
		char* mPos;

		// autocode	DO NOT EDIT!
        float getPosZ(){ return mPosZ;}
        void setPosZ(float value){ mPosZ = value;}

        int getA(int nIndex)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return 0;
            }
            return mA[nIndex];
        }
        void setA(int nIndex, int value)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return;
            }
            mA[nIndex] = value;
        }

        int getB(int nIndex, int nIndex2)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return 0;
            }
            if(nIndex2 < 0 || nIndex2 >= MAX_LENGTH2)
            {
                return 0;
            }
            return mB[nIndex][nIndex2];
        }
        void setB(int nIndex, int nIndex2, int value)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return;
            }
            if(nIndex2 < 0 || nIndex2 >= MAX_LENGTH2)
            {
                return;
            }
            mB[nIndex][nIndex2] = value;
        }

        char* getC(){ return mC;}
        void setC(const char* value)
        {
            if (NULL == value)
            {
                return;
            }
            strncpy(mC, value, sizeof(mC) - 1);
        }

        char* getD(int nIndex)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return 0;
            }
            return mD[nIndex];
        }
        void setD(int nIndex, char* value)
        {
            if(nIndex < 0 || nIndex >= MAX_LENGTH)
            {
                return;
            }
            strncpy(mD[nIndex], value, MAX_LENGTH2 - 1);
        }

        char* getPos(){ return mPos;}
        void setPos(char* value){ mPos = value;}
		// autocode
	};



private:
	SUB_CLASS_VECTOR		mSubClassList[MAX_LENGTH];		// 子类列表
	VARIABLE_VECTOR			mVariableList[MAX_LENGTH][MAX_LENGTH2];		// 变量列表
	CPlusClassTest*			mOutClass;			// 外层类
};