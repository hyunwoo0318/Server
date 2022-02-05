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

int main()
{
	ServerServiceRef service = make_shared<ServerService>(
		NetworkAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		make_shared<Session>,
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

