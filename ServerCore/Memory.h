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
		//-1024������ 32����, 1024~2048������ 128����, ~4096������ 256����
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

	//�޸� ũ��<-> �޸�Ǯ
	// 0(1) ������ã������ ���̺�
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
	//0~ 4096����Ʈ �߿��� ������ ã�� ���ؼ� ����.
};


template<typename Type, typename ...Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	//placement new
	new(memory) Type(forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	_xrelease(obj);
}