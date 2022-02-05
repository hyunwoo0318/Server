#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	Pre
	,
	Recv,
	Send,
};

/*----------
	IocpEvent
-------------*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);
	//만일 여기서 소멸자를 virtual로 선언하면 offset이 overlapped구조체가 아닌
	//다른 가상함수 테이블로 채워질수 있기때문에 virtual 선언은 하면 안된다.
	
	void Init();
	EventType GetType() { return eventType; }

public:
	EventType eventType;
	IocpObjectRef owner;
};

/*=--------------
	ConnectEvent
---------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};


/*=--------------
	AcceptEvent
---------------*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}




public:
	SessionRef session = nullptr;
	//TODO
};


/*=--------------
	RecvEvent
---------------*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};

/*=--------------
	SendEvent
---------------*/

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}
};


