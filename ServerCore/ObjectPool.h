#pragma once
#include"Types.h"
#include"MemoryPool.h"

//���ø����� �����ϸ� static�̿��� �ڷ������� �Ѱ����� static������ ������ �ȴ�.
template<typename Type>
class ObjectPool
{
public:
	template<typename...Args>
	static Type* Pop(Args&&... args)
	{
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_Pool.Pop(), s_allocSize));
		new(memory) Type(forward<Args>(args)...);
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->Type();
		s_Pool.Push(MemoryHeader::DetachHeader(obj));
	}

private:
	static int32 s_allocSize;
	static MemoryPool s_Pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_Pool{ s_allocSize };





