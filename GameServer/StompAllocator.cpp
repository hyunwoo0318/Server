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
//class Knight : public RefCountable
//{
//public:
//	Knight() { cout << "Knight" << endl; }
//	~Knight() { cout << "~knight" << endl; }
//
//	int32 _hp = 200;
//	int32 _mp = 100;
//	//static void* operator new(size_t size)
//	//{
//	//	cout << "New!" << size << endl;
//	//	void* ptr = ::malloc(size);
//	//	return ptr;
//	//}
//
//	//static void operator delete(void* ptr)
//	//{
//	//	cout << "Delete" << endl;
//	//	::free(ptr);
//	//}
//};

// new operator overloading(Global)

//
//int main()
//{
//	//Knight* k1 = new Knight();
//	//k1->_hp = 200;
//	//k1->_mp = 100;
//	//delete k1;
//	//k1 = nullptr;
//	//k1->_hp = 100; //Use-After-free 문제
//
//	//vector<int32> v{ 1,2,3,4,5 };
//	//for (int32 i = 0; i < 5; i++)
//	//{
//	//	int32 value = v[i];
//	//	if (value == 3)
//	//	{
//	//		v.clear();
//	//	}
//	//} // 충분히 발생할수있는 오류
//	////casting을 하면서 오류도 많이 생김.
//
//	////가상 메모리의 기본
//	//int32* num = new int;
//	//*num = 100;
//
//	//int64 address = reinterpret_cast<int64>(&num);
//	//cout << address << endl;
//
//	//delete num; //delete는 운영체제에게 가서 진짜로 메모리를 삭제하는것이 아닌 알아서 heap영역을 관리하기때문에
//	////crash가 날수있음.
//
//	////유저 레벨의 모든 프로그램은 서로에게 간섭하지않고 자기 자신만 실행된다고 생각함.
//
//	//int* test = (int*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//	//*test = 100;
//	//::VirtualFree(test,0, MEM_RELEASE);
//
//	//*test = 200; // crash가 나기때문에 오류를 찾기 훨씬 편함.
//
//
//	//Knight* knight = xnew<Knight>();
//	//xdelete(knight);
//
//	Knight* knight = xnew<Knight>(100);
//	xdelete(knight);
//	knight->_hp = 100;
//}