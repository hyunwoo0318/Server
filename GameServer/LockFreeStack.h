#pragma once

//---------------------
//1차 시도
//----------------------

/*
struct SListEntry
{
	SListEntry* next;
};

class Data //: public SListEntry
{
public:
	//리스트에서 data와 link를 따로두는 방식을 사용했는데
	//Data안에서 link까지 관리하는 Data의 형식을 만들어서 우리가 만든 데이터들은 사용할수있다.
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
//2차시도
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
//3차시도
//------------------

DECLSPEC_ALIGN(16) //16바이트로 정렬을 해줌
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
		//크게 align과 region이 있고 세부적으로 align에 depth, sequence가 있고 region에 reserved, next가 있다
		//이렇게 크게 접근할지 아니면 세부적으로 접근할지 두개의 방법을 주는 문법이 union이다.

	};
	
	SListEntry* next = nullptr;
};

void InitialzeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);