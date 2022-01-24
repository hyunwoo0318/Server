#pragma once

/*-----------
 BaseAllocator
-----------*/
class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};


/*----------------
 StompAllocator
-----------------*/
//버그를 잡는데 굉장히 유용함

class StompAllocator
{
	enum {PAGE_SIZE = 0X1000};
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};

/*----------------
 PoolAllocator
-----------------*/


class PoolAllocator
{
	enum { PAGE_SIZE = 0X1000 };
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};


/*----------------
 STLAllocator
-----------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename U>
	StlAllocator(const StlAllocator<U>&) {}

	T* allocate(size_t count) // 컨테이너에서의 원소의 개수 = count
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xalloc(size));
	}
	void deadlocate(T* ptr, size_t count)
	{
		_xrelease(ptr);
	}
};
