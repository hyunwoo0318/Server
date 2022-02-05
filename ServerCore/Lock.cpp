#include "pch.h"
#include "Lock.h"
#include"CoreTLS.h"
#include"DeadLockProfiler.h"

//표준 mutex는 재귀적으로 lock이 걸리지않음
//매우 낮은 확률로 변경이 있고 일정한 경우에도 계속 lock을 거는것은
//굉장히 아까움 -> Reader write lock
//--평소에는 모든 쓰레드들이 같이 쓰기만 하지만 write를 하는경우
//상호배타적으로 한번에 한명만 lock을 걸어 사용한다.

void Lock::WriteLock(const char* name)
{

#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	//재귀적으로 동일한 쓰레드가 소유하고 있다면 무조건 성공.
	const uint32 LockThreadId = (_lockflag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == LockThreadId)
	{
		_writecount++;
		return;
	}

	//아무도 소유 및 공유하고 있지 않을때, 경합해서 소유권을 얻음
	//-- 모든 값이 empty flag인 경우임.
	//if (_lockflag == EMPTY_FLAG)
	//{
	//	const int32 desired = ((LThreadId << 16) && WRITE_THREAD_MASK);
	//	_lockflag = desired;
	//	// 멀티쓰레드 환경에서는 중간에 누가 쓸수 있기때문에 따로해야함
	//}
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (1)
	{
		for (uint32 Spincount = 0; Spincount < MAX_SPIN_COUNT; Spincount++)
		{
			uint32 expected = EMPTY_FLAG; //계속 초기화를 해줘야함.
			if (_lockflag.compare_exchange_strong(OUT expected, desired))
			{
				_writecount++;
				return;
			}
		}
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Lock_timeout");
		this_thread::yield();
	}

}

void Lock::WriteUnlock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	//ReadLock을 풀기 전에는 WriteLock이 불가능
	if ((_lockflag.load() & READ_COUNT_MASK) != 0)
		CRASH("invalid unlock order");

	const int32 lockCount = --_writecount;
	if (lockCount == 0)
	{
		_lockflag.store(EMPTY_FLAG);
	}

}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	//동일한 쓰레드가 소유하고있으면 무조건 성공
	const uint32 LockThreadId = (_lockflag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == LockThreadId)
	{
		_lockflag.fetch_add(1);
		return;
	}

	//아무도 소유하고 있지 않을때(누가 write를 하지않을때) 경합해서 공유 카운트를 올린다.
	const int64 beginTick = ::GetTickCount64();

	while (1)
	{
		for (uint32 Spincount = 0; Spincount < MAX_SPIN_COUNT; Spincount++)
		{
			uint32 expected = (_lockflag.load() & READ_COUNT_MASK);
			if (_lockflag.compare_exchange_strong(OUT expected, expected+1))
			{
				return;
			}
			//실패의 경우 - 누가 writelock을 한 경우, 누가 먼저 해버려서
			//expected값이 변하는 경우

			if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
				CRASH("Lock_timeout");

			this_thread::yield();
		}
	}
}

void Lock::ReadUnlock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	if ((_lockflag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("ERROR");
}
