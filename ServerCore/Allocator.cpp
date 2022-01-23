#include "pch.h"
#include "Allocator.h"
#include"Memory.h"

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}


/*----------------
 StompAllocator
-----------------*/

//memory overflow문제를 해결하는데 좋음.

void* StompAllocator::Alloc(int32 size)
{
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;
	//할당된 크기 - 실제로 사용할 크기 = 남는 여분의 공간

	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
	//[[]      ] -->> [        []] overflow문제 해결
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

/*----------------
 PoolAllocator
-----------------*/

void* PoolAllocator::Alloc(int size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemory->Release(ptr);
}
