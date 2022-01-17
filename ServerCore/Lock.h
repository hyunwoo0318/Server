#pragma once
#include "Types.h"

/*-----------------
	RW Spinlock
-----------------*/

/* --------------
[wwwwwwww][wwwwwwww][rrrrrrrr][rrrrrrrr]
-- ���� 2����Ʈ�� readflag -  Shared Lock Count
--���� 2����Ʈ�� writeflag -- Exclusive Lock Owner ThreadId
----------------*/

//W->R(O) ������
//R->W(x) �Ұ���
class Lock
{

	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000, // �ִ�� ��ٸ� �ð�
		MAX_SPIN_COUNT = 5000, // �ִ� ����� ��������
		WRITE_THREAD_MASK = 0xFFFF'0000,
		//���� 16��Ʈ�� �̾Ƴ��� ���� ����ũ ����
		READ_COUNT_MASK = 0x0000'FFFF,
		//���� 16��Ʈ�� �̾Ƴ��� ���� ����ũ ����
		EMPTY_FLAG = 0x0000'0000
		//ó�� ���¸� ��Ÿ���� ����ũ ����
		
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

//RAII��� -- �����ɶ� lock�� ��� �Ҹ�ɶ� lock�� ������.
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