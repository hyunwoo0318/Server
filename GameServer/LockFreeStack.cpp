#include "pch.h"
#include "LockFreeStack.h"

/*
void InitialzeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;
	if(!first)
	{
	header->next = first->next;
	}
	
	return first;
}
*/

//void InitialzeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	//두번째 인자가 desired이고 세번째 인자가 expeceted이다.
//	while (::InterlockedCompareExchange64((int64*) & header->next, (int64)entry, (int64)entry->next) == 0)
//	{
//	}
//}
//
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* expected = header->next;
//
//	//ABA problem
//	//a->b->a[5]->[6]->[7]인 상황을 가정했을때 만약 pop을 해서 5,6이 날라가고 우연히 push로 5가 들어온 경우에
//	//우리는 5를 expected 6을 desired로 둔것이여서 5를 pop하고 6을 header로 만드는 상황이 발생한다 -- 이것이aba문제
//	// data에 다른 counter값을 넣어서 데이터를 구별할수있다.
//	
//	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
//	{
//
//	}
//
//	return expected;
//}

void InitialzeHead(SListHeader* header)
{
	header->alignment = 0;
	header->region = 0;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};
	
	//16비트로 align하면 하위 4비트가 무조건 0000이여서 60비트로 수를 표현할수있다.
	desired.HeaderX64.next = (((uint64)entry) >> 4);
	
	while (1)
	{
		expected = *header;
		//이 사이에 변경될 수 있다.
		entry->next = (SListEntry*)((uint64)expected.HeaderX64.next << 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (1)
	{
		expected = *header;

		entry = (SListEntry*)((uint64)expected.HeaderX64.next << 4);
		if (entry == nullptr)
			break;
		desired.HeaderX64.next = (((uint64)expected.HeaderX64.next) >> 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if(::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected )== 1)
			break;
	}
}