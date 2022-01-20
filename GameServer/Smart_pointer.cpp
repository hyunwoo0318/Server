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
//	//직접 만드는 TsharedPtr의 단점
//	//1) 이미 만들어진 클래스 대사으로 사용이 불가함
//	//2) 순환(cycle) 문제
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
//	// 복사를 막아놓은 점만 빼면은 일반 포인터와 같음.
//
//	//[Knight][RefCountingBlock(uses, weak)] 2개의 정수를 관리함.
//
//	//[T*][RefCountBlock*]
//	//[Knight][RefCountingBlock]
//	//RefCountBlock(useCount(shared), weakCount)
//	shared_ptr<Knight> spr(new Knight());
//	//[Knight || RefCountingBlock] 이러한 차이가있음.
//	//동적할당을 해야하는 횟수의 차이가 있으므로 make_shared가 더 좋은 방법이다.
//	shared_ptr<Knight> spr2 = make_shared<Knight>();
//	weak_ptr<Knight> wpr = spr;
//	// weak_ptr은 Knight블록에는 영향을 주지않는 반쪽짜리 포인터이다.
//
//	//일단 존재하는지 확인을 해야함
//	bool expired = wpr.expired();
//
//	//만일 spr이 없으면 spr3에 null이 들어감.
//	shared_ptr<Knight> spr3 = wpr.lock();
//}