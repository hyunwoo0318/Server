#include "pch.h"
#include "MemoryPool.h"

/*---------------
	MemoryPool
----------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocsize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	
	//pool에 메모리 반납
	_queue.push(ptr);

	_allocCount.fetch_add(-1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	WRITE_LOCK;
	//Pool에 여분이 있는지?
	if (_queue.empty() == false)
	{
		header = _queue.front();
		_queue.pop();
	}

	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocsize));
	}
	else
	{
		ASSERT_CRASH(header->allocSize == 0)
	}
	
	_allocCount.fetch_add(1);
	return header;
	
}
