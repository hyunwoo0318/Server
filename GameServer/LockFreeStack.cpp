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
//	//�ι�° ���ڰ� desired�̰� ����° ���ڰ� expeceted�̴�.
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
//	//a->b->a[5]->[6]->[7]�� ��Ȳ�� ���������� ���� pop�� �ؼ� 5,6�� ���󰡰� �쿬�� push�� 5�� ���� ��쿡
//	//�츮�� 5�� expected 6�� desired�� �а��̿��� 5�� pop�ϰ� 6�� header�� ����� ��Ȳ�� �߻��Ѵ� -- �̰���aba����
//	// data�� �ٸ� counter���� �־ �����͸� �����Ҽ��ִ�.
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
	
	//16��Ʈ�� align�ϸ� ���� 4��Ʈ�� ������ 0000�̿��� 60��Ʈ�� ���� ǥ���Ҽ��ִ�.
	desired.HeaderX64.next = (((uint64)entry) >> 4);
	
	while (1)
	{
		expected = *header;
		//�� ���̿� ����� �� �ִ�.
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