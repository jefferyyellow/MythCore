#include "commontype.h"
#include "variable.h"
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
		}
	public:
		// default: ignore
		/* int mPosX;*/
		// default: ignore
		int mPosY;
		// λ�� default: -1
		float mPosZ;
		int a[10];
		char c[10];
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
	SUB_CLASS_VECTOR		mSubClassList;		// �����б�
	VARIABLE_VECTOR			mVariableList;		// �����б�
	CPlusClass*				mOutClass;			// �����
};