#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include<WinSock2.h>
#include<MSWSock.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include"SocketUtils.h"
#include"Listener.h"
#include"session.h"
#include"service.h"

class GameSession : public Session
{
public:
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		//Echo
		cout << "OnRecv Len" << endl;
		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "onsend Len" << endl;
	}
};

int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetworkAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);
		
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]() {
			while (1)
			{
				service->GetIocpCore()->Dispatch();
			}
			});
	}

	
	GThreadManager->Join();
	
}

