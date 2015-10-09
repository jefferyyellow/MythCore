#ifndef __SINGLETON_H__
#define __SINGLETON_H__

/**
 * Singleton design pattern
 * 
 * for instance
 * \code 
	// class declaration
	class CTimeManager : public CSingleton<CTimeManager>
	{
		private:
			CTimeManager();
			~CTimeManager();
	};

	// call CreateInst before first use
	CTimeManager::CreateInst();
	...
	sint64 tNow = CTimeManager::Inst()->GetMSTime();
	...
	// call DestroyInst if nerver use CTimeManager
	CTimeManager::DestroyInst();
 * \endcode 
 * \author jefferyyellow
 * \date 2014
 */


template<typename T>
class CSingleton
{
public:
	CSingleton()
	{
		mInst = static_cast<T*>(this);
	};
	~CSingleton(){}
	
public:
	/// get the singleton instance
	static T*	Inst()
	{
		return mInst;
	}
	/// create the singleton instance
	static T*   CreateInst()
	{
		if (mInst == NULL)
		{
			return new T;
		}
		return mInst;
	}
	/// destroy the singleton instance
	static void DestroyInst()
	{
		if (NULL != mInst)
		{
			delete mInst;
		}
	}
private:
	static T*	mInst;
};

template< typename T >
T* CSingleton<T>::mInst = NULL;

//#define NEW_INST(SingletonClass) \
//	static SingletonClass* NewInst() \
//	{ \
//		if (NULL == SingletonClass::Inst()) \
//		{ \
//			return new SingletonClass; \
//		} \
//		return SingletonClass::Inst(); \
//	}
//
//#define DESTROY_INST(SingletonClass) \
//	static void DestroyInst() \
//	{ \
//		if (NULL != SingletonClass::Inst()) \
//		{ \
//			delete SingletonClass::Inst(); \
//		} \
//	}

#endif