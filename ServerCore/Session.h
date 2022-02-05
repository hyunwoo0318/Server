#pragma once
#include"IocpCore.h"
#include"IocpEvent.h"
#include"NetworkAddress.h"

/*-------------
	Session
-------------*/

class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/*���� ����*/
	void SetNetAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress GetNetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }

public:
	/*�������̽� ����*/
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;


public:
	//TEMP
	char _recvBuffer[1000];

private:
	SOCKET _socket = INVALID_SOCKET;
	NetworkAddress _netAddress = {};
	Atomic<bool>	_connected = false;
};

