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
	//���� ���⼭ �Ҹ��ڸ� virtual�� �����ϸ� offset�� overlapped����ü�� �ƴ�
	//�ٸ� �����Լ� ���̺�� ä������ �ֱ⶧���� virtual ������ �ϸ� �ȵȴ�.
	
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


