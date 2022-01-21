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
	//�ι�° ���ڰ� desired�̰� ����° ���ڰ� expeceted�̴�.
	while (::InterlockedCompareExchange64((int64*) & header->next, (int64)entry, (int64)entry->next) == 0)
	{
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* expected = header->next;

	//ABA problem
	//a->b->a[5]->[6]->[7]�� ��Ȳ�� ���������� ���� pop�� �ؼ� 5,6�� ���󰡰� �쿬�� push�� 5�� ���� ��쿡
	//�츮�� 5�� expected 6�� desired�� �а��̿��� 5�� pop�ϰ� 6�� header�� ����� ��Ȳ�� �߻��Ѵ� -- �̰���aba����

	
	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
	{

	}

	return expected;
}