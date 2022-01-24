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
//���׸� ��µ� ������ ������

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

	T* allocate(size_t count) // �����̳ʿ����� ������ ���� = count
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xalloc(size));
	}
	void deadlocate(T* ptr, size_t count)
	{
		_xrelease(ptr);
	}
};
