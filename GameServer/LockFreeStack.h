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
