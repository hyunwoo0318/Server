#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

//소수 구하기 문제

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;
		if (_queue.empty())
			return -1;
		else return _queue.front();
	}

	void TestPop()
	{
		WRITE_LOCK;
		
		if (_queue.empty() == false)
			_queue.pop();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}
private:
	queue<int32> _queue;
};

TestLock testlock;

void ThreadWrite()
{
	while (1)
	{
		testlock.TestPush();
		this_thread::sleep_for(1ms);
		testlock.TestPop();
	}
}

void ThreadRead()
{
	while (1)
	{
		int32 value = testlock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadRead);
	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	GThreadManager->Join();
}