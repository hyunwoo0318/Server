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
//	//k1->_hp = 100; //Use-After-free ����
//
//	//vector<int32> v{ 1,2,3,4,5 };
//	//for (int32 i = 0; i < 5; i++)
//	//{
//	//	int32 value = v[i];
//	//	if (value == 3)
//	//	{
//	//		v.clear();
//	//	}
//	//} // ����� �߻��Ҽ��ִ� ����
//	////casting�� �ϸ鼭 ������ ���� ����.
//
//	////���� �޸��� �⺻
//	//int32* num = new int;
//	//*num = 100;
//
//	//int64 address = reinterpret_cast<int64>(&num);
//	//cout << address << endl;
//
//	//delete num; //delete�� �ü������ ���� ��¥�� �޸𸮸� �����ϴ°��� �ƴ� �˾Ƽ� heap������ �����ϱ⶧����
//	////crash�� ��������.
//
//	////���� ������ ��� ���α׷��� ���ο��� ���������ʰ� �ڱ� �ڽŸ� ����ȴٰ� ������.
//
//	//int* test = (int*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//	//*test = 100;
//	//::VirtualFree(test,0, MEM_RELEASE);
//
//	//*test = 200; // crash�� ���⶧���� ������ ã�� �ξ� ����.
//
//
//	//Knight* knight = xnew<Knight>();
//	//xdelete(knight);
//
//	Knight* knight = xnew<Knight>(100);
//	xdelete(knight);
//	knight->_hp = 100;
//}