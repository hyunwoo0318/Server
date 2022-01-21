#pragma once

//---------------------
//1�� �õ�
//----------------------

/*
struct SListEntry
{
	SListEntry* next;
};

class Data //: public SListEntry
{
public:
	//����Ʈ���� data�� link�� ���εδ� ����� ����ߴµ�
	//Data�ȿ��� link���� �����ϴ� Data�� ������ ���� �츮�� ���� �����͵��� ����Ҽ��ִ�.
	SListEntry _entry;

	int32 _hp;
	int32 _mp;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};
//[header] -> [] ->[] ->[][]

void InitialzeHead(SListHeader* header);

void PushEntrySList(SListHeader* header, SListEntry* entry);

SListEntry* PopEntrySList(SListHeader* header);
*/

//-------------
//2���õ�
//------------------

struct SListEntry
{
	SListEntry* next;
};

class Data //: public SListEntry
{
public:
	
	SListEntry _entry;

	int32 _hp;
	int32 _mp;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};
void InitialzeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
