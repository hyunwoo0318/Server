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

void InitialzeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	//두번째 인자가 desired이고 세번째 인자가 expeceted이다.
	while (::InterlockedCompareExchange64((int64*) & header->next, (int64)entry, (int64)entry->next) == 0)
	{
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* expected = header->next;

	//ABA problem
	//a->b->a[5]->[6]->[7]인 상황을 가정했을때 만약 pop을 해서 5,6이 날라가고 우연히 push로 5가 들어온 경우에
	//우리는 5를 expected 6을 desired로 둔것이여서 5를 pop하고 6을 header로 만드는 상황이 발생한다 -- 이것이aba문제

	
	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
	{

	}

	return expected;
}