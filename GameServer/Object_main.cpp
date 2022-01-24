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
//	//����Ʈ���� data�� link�� ���εδ� ����� ����ߴµ�
//	//Data�ȿ��� link���� �����ϴ� Data�� ������ ���� �츮�� ���� �����͵��� ����Ҽ��ִ�.
//	SLIST_ENTRY _entry;
//	int64 _rand = rand() % 100;
//};
//
//SLIST_HEADER* GHeader;
//
//int main()
//{
//	//ù��° ���ڴ� ����, �ι�° ���ڴ� ������Ű�� ����̴�.
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