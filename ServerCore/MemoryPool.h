#pragma once

//[32][64][][][][]

//[32 32 32 32 ] [100 200]

/*---------------
	MemoryHeader
----------------*/

struct MemoryHeader
{
	//[MemoryHeader][Data]
	int32 allocSize;
	MemoryHeader(int32 size) : allocSize(size) {}

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); //placement new
		return reinterpret_cast<void*>(++header);
		//data의 시작 위치를 반환함
	}
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
	//TODO : 필요한 추가 정보

};

/*---------------
	MemoryPool
----------------*/

class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	int32 _allocsize = 0;
	atomic<int32> _allocCount = 0;

	USE_LOCK;
	queue<MemoryHeader*> _queue;
};

