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

//class Knight
//{
//public:
//	int32 _hp = rand() % 1000;
//};
//
//DECLSPEC_ALIGN(16)
//class Data //: public SListEntry
//{
//public:
//	//리스트에서 data와 link를 따로두는 방식을 사용했는데
//	//Data안에서 link까지 관리하는 Data의 형식을 만들어서 우리가 만든 데이터들은 사용할수있다.
//	SLIST_ENTRY _entry;
//	int64 _rand = rand() % 100;
//};
//
//SLIST_HEADER* GHeader;
//
//int main()
//{
//	//첫번째 인자는 생성, 두번째 인자는 삭제시키는 방법이다.
//	shared_ptr<Knight> sptr = { ObjectPool<Knight>::Pop(), ObjectPool<Knight>::Push };
//
//	GHeader = new SLIST_HEADER();
//	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
//	::InitializeSListHead(GHeader);
//
//	for (int32 i = 0; i < 3; i++)
//	{
//		GThreadManager->Launch([]()
//			{
//				while (1)
//				{
//					Data* data = new Data;
//					ASSERT_CRASH(((uint64)GHeader % 16) == 0);
//
//					::InterlockedPushEntrySList(GHeader, (SLIST_ENTRY*)data);
//					this_thread::sleep_for(10ms);
//				}
//
//
//			});
//	}
//
//	for (int32 i = 0; i < 3; i++)
//	{
//		GThreadManager->Launch([]()
//			{
//				while (1)
//				{
//					Data* pop = nullptr;
//					pop = (Data*)::InterlockedPopEntrySList(GHeader);
//
//					if (pop)
//					{
//						cout << pop->_rand << endl;
//						delete pop;
//					}
//					else
//					{
//						cout << "none" << endl;
//					}
//				}
//
//
//			});
//	}
//
//	GThreadManager->Join();
//}