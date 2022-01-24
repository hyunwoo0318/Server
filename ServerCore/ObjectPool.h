#pragma once
#include"Types.h"
#include"MemoryPool.h"

//템플릿으로 선언하면 static이여도 자료형별로 한개씩의 static변수를 가지게 된다.
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





