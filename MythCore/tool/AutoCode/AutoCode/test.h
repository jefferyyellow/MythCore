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

	}
	// autocode	DO NOT EDIT!
	// autocode
	class CNestClassTest
	{
	public:
		void init()
		{
		}
	public:
		// default: ignore
		/* int mPosX;*/
		// default: ignore
		int mPosY;
		// λ�� default: -1
		float mPosZ;
		int mA[MAX_LENGTH];
		int mB[MAX_LENGTH][MAX_LENGTH2];
		char mC[MAX_LENGTH];
		char mD[MAX_LENGTH][MAX_LENGTH2];
		char* mPos;

		// autocode	DO NOT EDIT!
		// autocode
	};



private:
	SUB_CLASS_VECTOR		mSubClassList[MAX_LENGTH];		// �����б�
	VARIABLE_VECTOR			mVariableList[MAX_LENGTH][MAX_LENGTH2];		// �����б�
	CPlusClassTest*			mOutClass;			// �����
};