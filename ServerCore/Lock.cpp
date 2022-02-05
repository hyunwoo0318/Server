#include "pch.h"
#include "Lock.h"
#include"CoreTLS.h"
#include"DeadLockProfiler.h"

//ǥ�� mutex�� ��������� lock�� �ɸ�������
//�ſ� ���� Ȯ���� ������ �ְ� ������ ��쿡�� ��� lock�� �Ŵ°���
//������ �Ʊ�� -> Reader write lock
//--��ҿ��� ��� ��������� ���� ���⸸ ������ write�� �ϴ°��
//��ȣ��Ÿ������ �ѹ��� �Ѹ� lock�� �ɾ� ����Ѵ�.

void Lock::WriteLock(const char* name)
{

#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	//��������� ������ �����尡 �����ϰ� �ִٸ� ������ ����.
	const uint32 LockThreadId = (_lockflag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == LockThreadId)
	{
		_writecount++;
		return;
	}

	//�ƹ��� ���� �� �����ϰ� ���� ������, �����ؼ� �������� ����
	//-- ��� ���� empty flag�� �����.
	//if (_lockflag == EMPTY_FLAG)
	//{
	//	const int32 desired = ((LThreadId << 16) && WRITE_THREAD_MASK);
	//	_lockflag = desired;
	//	// ��Ƽ������ ȯ�濡���� �߰��� ���� ���� �ֱ⶧���� �����ؾ���
	//}
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (1)
	{
		for (uint32 Spincount = 0; Spincount < MAX_SPIN_COUNT; Spincount++)
		{
			uint32 expected = EMPTY_FLAG; //��� �ʱ�ȭ�� �������.
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
	//ReadLock�� Ǯ�� ������ WriteLock�� �Ұ���
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
	//������ �����尡 �����ϰ������� ������ ����
	const uint32 LockThreadId = (_lockflag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == LockThreadId)
	{
		_lockflag.fetch_add(1);
		return;
	}

	//�ƹ��� �����ϰ� ���� ������(���� write�� ����������) �����ؼ� ���� ī��Ʈ�� �ø���.
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
			//������ ��� - ���� writelock�� �� ���, ���� ���� �ع�����
			//expected���� ���ϴ� ���

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
