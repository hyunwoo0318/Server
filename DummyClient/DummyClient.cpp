#include "pch.h"
#include <iostream>
#include"service.h"
#include"session.h"
#include "ThreadManager.h"

char sendBuffer[] = "Hello World";

class ClientSession : public Session
{
public:

	virtual void OnConnected() override
	{
		cout << "connected to server" << endl;
		Send((BYTE*)sendBuffer, sizeof(sendBuffer));
	}
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		//Echo
		cout << "OnRecv Len" << endl;
		this_thread::sleep_for(1s);
		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "onsend Len" << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "disconnected" << endl;
	}
};

int main()
{
	this_thread::sleep_for(1s);
		
	ClientServiceRef service = MakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ClientSession>,
		1);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]() {
			while (1)
			{
				service->GetIocpCore()->Dispatch();
			}
			});
	}
}