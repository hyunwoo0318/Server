#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"

class Wraith: public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
};

class Missile : public RefCountable
{
public:
	void SetTarget(WraithRef target)
	{
		_target = target;
		//중간에 개입 가능 -> 멀티쓰레드
		//target->AddRef();
	}
	

	bool Update()
	{
		if (_target == nullptr)
			return true;

		int PosX = _target->_posX;
		int PosY = _target->_posY;

		if (_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return true;
		}
		return false;
	}

	WraithRef _target = nullptr;
};

using WraithRef = TsharedPtr<Wraith>;
using MissileRef = TsharedPtr<Missile>;

int main()
{
	WraithRef wraith = new Wraith();
	wraith->ReleaseRef();
	MissileRef missile = new Missile();
	missile->ReleaseRef();

	missile->SetTarget(wraith);

	wraith->_hp = 0;
	//delete wraith;
	//wraith->ReleaseRef();
	wraith = nullptr;

	while (1)
	{
		if (missile.IsNull())
		{
			missile->Update();
		}
	}
	return 0;
	//missile->ReleaseRef();
	missile = nullptr;
	//delete missile;
}