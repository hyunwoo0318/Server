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
//
//class Wraith : public RefCountable
//{
//public:
//	int _hp = 150;
//	int _posX = 0;
//	int _posY = 0;
//};
//
//using WraithRef = TsharedPtr<Wraith>;
//class Missile : public RefCountable
//{
//public:
//	void SetTarget(WraithRef target)
//	{
//		_target = target;
//		//�߰��� ���� ���� -> ��Ƽ������
//		//target->AddRef();
//	}
//
//
//	bool Update()
//	{
//		if (_target == nullptr)
//			return true;
//
//		int PosX = _target->_posX;
//		int PosY = _target->_posY;
//
//		if (_target->_hp == 0)
//		{
//			_target->ReleaseRef();
//			_target = nullptr;
//			return true;
//		}
//		return false;
//	}
//
//	WraithRef _target = nullptr;
//};
//
//using MissileRef = TsharedPtr<Missile>;
//
//int main()
//{
//	WraithRef wraith = new Wraith();
//	wraith->ReleaseRef();
//	//_refCount�� 1�� �Ǹ鼭 ��� ������ ����Ʈ�����Ϳ��� �ش�.
//	MissileRef missile = new Missile();
//	missile->ReleaseRef();
//
//	missile->SetTarget(wraith);
//
//	wraith->_hp = 0;
//	//delete wraith;
//	//wraith->ReleaseRef();
//	wraith = nullptr;
//
//	while (!(missile->Update()))
//	{
//	}
//	//missile->ReleaseRef();
//	missile = nullptr;
//	//delete missile;
//}