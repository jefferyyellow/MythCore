#include "commontype.h"
#include "variable.h"
#define  MAX_LENGTH 10
class CPlusClass
{
public:
	CPlusClass()
	{
		mOutClass = NULL;
	}
	~CPlusClass(){}

	void init();

	class CNestClass
	{
	public:
		void init()
		{
            mPosZ = -1;
            memset(mA, 0, sizeof(mA));
            mC[0] = '\0';
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
		char mC[10];
		char* mPos;

		// autocode	DO NOT EDIT!
        int getPosY(){ return mPosY}
        void setPosY(int value){ mPosY = value;}

        float getPosZ(){ return mPosZ}
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

        char* getC(){ return mC}
        void setC(const char* value)
        {
            if (NULL == value)
            {
                return;
            }
            strncpy(mC, value, sizeof(mC) - 1);
        }

        char* getPos(){ return mPos}
        void setPos(char* value){ mPos = value;}
		// autocode
	};


public:
	typedef vector<CPlusClass*>		SUB_CLASS_VECTOR;
	typedef vector<CVariable*>		VARIABLE_VECTOR;

public:
	SUB_CLASS_VECTOR&		getSubClassList(){ return mSubClassList; }
	VARIABLE_VECTOR&		getVariableList(){ return mVariableList; }

	CPlusClass* getOutClass() const { return mOutClass; }
	void setOutClass(CPlusClass* val) { mOutClass = val; }

private:
	SUB_CLASS_VECTOR		mSubClassList;		// 子类列表
	VARIABLE_VECTOR			mVariableList;		// 变量列表
	CPlusClass*				mOutClass;			// 外层类
};