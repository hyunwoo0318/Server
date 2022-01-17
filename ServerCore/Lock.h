#pragma once
#include "Types.h"

/*-----------------
	RW Spinlock
-----------------*/

/* --------------
[wwwwwwww][wwwwwwww][rrrrrrrr][rrrrrrrr]
-- 상위 2바이트는 readflag -  Shared Lock Count
--하위 2바이트는 writeflag -- Exclusive Lock Owner ThreadId
----------------*/

//W->R(O) 가능함
//R->W(x) 불가능
class Lock
{

	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000, // 최대로 기다릴 시간
		MAX_SPIN_COUNT = 5000, // 최대 몇바퀴 돌릴껀지
		WRITE_THREAD_MASK = 0xFFFF'0000,
		//상위 16비트를 뽑아내기 위한 마스크 설정
		READ_COUNT_MASK = 0x0000'FFFF,
		//하위 16비트를 뽑아내기 위한 마스크 설정
		EMPTY_FLAG = 0x0000'0000
		//처음 상태를 나타내는 마스크 설정
		
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);
private:
	Atomic<uint32> _lockflag = EMPTY_FLAG;
	uint16 _writecount = 0;
};

/*-------------
	LOCK GUARD
-----------------*/

//RAII방식 -- 생성될때 lock을 잡고 소멸될때 lock을 해제함.
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock,const char *name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;

};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;

};