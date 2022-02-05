#include "pch.h"
#include "IocpCore.h"
#include"IocpEvent.h"



IocpCore::IocpCore()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	
	return ::CreateIoCompletionPort(iocpObject->GetHandle() , _iocpHandle,
		/*key*/0, 0);
	
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	
	IocpEvent* iocpEvent = nullptr;
	ULONG_PTR key = 0;

	if (::GetQueuedCompletionStatus(_iocpHandle, &numOfBytes, &key,
		reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{	IocpObjectRef iocpObject = iocpEvent->owner;
	iocpObject->Dispatch(iocpEvent, numOfBytes);
		
	
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
				return false;
			default:
				//TODO : �α����
				IocpObjectRef iocpObject = iocpEvent->owner;
				iocpObject->Dispatch(iocpEvent, numOfBytes);
				break;
		}
	}
	return false;
}
