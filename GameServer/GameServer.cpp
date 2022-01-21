#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"
#include"Memory.h"
#include"LockFreeStack.h"

class Data //: public SListEntry
{
public:
	//리스트에서 data와 link를 따로두는 방식을 사용했는데
	//Data안에서 link까지 관리하는 Data의 형식을 만들어서 우리가 만든 데이터들은 사용할수있다.
	SListEntry _entry;

	int32 _hp;
	int32 _mp;
};

int main()
{
	SListHeader header;
	InitialzeHead(&header);

	Data* data = new Data();
	data->_hp = 10;
	data->_mp = 20;
	PushEntrySList(&header, (SListEntry*)data);

	Data* popData = (Data*)PopEntrySList(&header);
}