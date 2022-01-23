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

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//class Data //: public SListEntry
//{
//public:
//	
//	SListEntry _entry;
//
//	int32 _hp;
//	int32 _mp;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//void InitialzeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

//-------------
//3���õ�
//------------------

DECLSPEC_ALIGN(16) //16����Ʈ�� ������ ����
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}
	union
	{
		struct
		{
			uint64  alignment;
			uint64 region;
		}DUMMYSTRUCTNAME;
		struct
		{
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		}HeaderX64;
		//ũ�� align�� region�� �ְ� ���������� align�� depth, sequence�� �ְ� region�� reserved, next�� �ִ�
		//�̷��� ũ�� �������� �ƴϸ� ���������� �������� �ΰ��� ����� �ִ� ������ union�̴�.

	};
	
	SListEntry* next = nullptr;
};

void InitialzeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);