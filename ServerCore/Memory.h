#pragma once
#include"Allocator.h"

class MemoryPool;


/*---------------
	Memory
----------------*/

class Memory
{
	enum
	{
		//-1024까지는 32단위, 1024~2048까지는 128단위, ~4096까지는 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 138) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	//메모리 크기<-> 메모리풀
	// 0(1) 빠르게찾기위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
	//0~ 4096바이트 중에서 빠르게 찾기 위해서 만듬.
};


template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	PoolAllocator::Release(obj);
}

template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
}