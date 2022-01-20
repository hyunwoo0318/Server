#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"
//
//using KnightRef = TsharedPtr<class Knight>;
//
//class Knight : public RefCountable
//{
//public:
//	Knight() { cout << "Knight" << endl; }
//	~Knight() { cout << "~knight" << endl; }
//
//	void SetTarget(KnightRef target)
//	{
//		_target = target;
//	}
//
//	KnightRef _target = nullptr;
//};
//
//int main()
//{
//	//���� ����� TsharedPtr�� ����
//	//1) �̹� ������� Ŭ���� ������� ����� �Ұ���
//	//2) ��ȯ(cycle) ����
//
//	KnightRef k1(new Knight());
//	k1->ReleaseRef();
//	KnightRef k2(new Knight());
//	k2->ReleaseRef();
//
//	k1->SetTarget(k2);
//	k2->SetTarget(k1);
//
//	k1 = nullptr;
//	k2 = nullptr;
//
//	//unique_ptr, shared_ptr, weak_ptr
//	unique_ptr<Knight> k2 = make_unique<Knight>();
//	// ���縦 ���Ƴ��� ���� ������ �Ϲ� �����Ϳ� ����.
//
//	//[Knight][RefCountingBlock(uses, weak)] 2���� ������ ������.
//
//	//[T*][RefCountBlock*]
//	//[Knight][RefCountingBlock]
//	//RefCountBlock(useCount(shared), weakCount)
//	shared_ptr<Knight> spr(new Knight());
//	//[Knight || RefCountingBlock] �̷��� ���̰�����.
//	//�����Ҵ��� �ؾ��ϴ� Ƚ���� ���̰� �����Ƿ� make_shared�� �� ���� ����̴�.
//	shared_ptr<Knight> spr2 = make_shared<Knight>();
//	weak_ptr<Knight> wpr = spr;
//	// weak_ptr�� Knight��Ͽ��� ������ �����ʴ� ����¥�� �������̴�.
//
//	//�ϴ� �����ϴ��� Ȯ���� �ؾ���
//	bool expired = wpr.expired();
//
//	//���� spr�� ������ spr3�� null�� ��.
//	shared_ptr<Knight> spr3 = wpr.lock();
//}