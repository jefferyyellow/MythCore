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
		/* int mPosX;
		int mPosY;*/
		// 位置 default: -1
		float mPosZ;
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