#ifndef __MEMORY_H__
#define __MEMORY_H__

/// Get the class member memory offset byte from the class(instance) head
#define CLASS_MEM_OFFSET(Class,Member) ((int)(&((Class*)0)->Member))

template<typename T, bool>
class comconstruct
{
public:
	inline static void construct(void* p)
	{
		new (p)T;
	}

	inline static void construct(void* p, const T& t)
	{
		/*new (p)T(t);*/
		new (p) T(t);
	}
};

template<typename T>
class comconstruct<T, false>
{
public:
	inline static void construct(void* p)
	{
		new (p)T;
	}

	inline static void construct(void* p, const T& t)
	{
		new (p)T(t);
	}
};

template<typename T, bool>
class comdeconstruct
{
public:
	inline static void deconstruct(T* p)
	{
	}
};

template<typename T>
class comdeconstruct<T, false>
{
public:
	inline static void deconstruct(T* p)
	{
		p->~T();
	}
};
#endif