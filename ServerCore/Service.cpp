#include "pch.h"
#include "Service.h"
#include"session.h"
#include"listener.h"

/*------------
	Service
-------------*/

Service::Service(ServiceType type, NetworkAddress address, IocpObjectRef core, SessionFactory factory, int32 maxSessionCount)
{
}

void Service::CloseService()
{
	//TODO
}

SessionRef Service::CreateSession()
{
	SessionRef session;
	if (_iocpCore->Register(session) == false)
		return nullptr;
	return session;
}

void Service::AddSession(SessionRef session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);

}

void Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
	

}

ClientService::ClientService(NetworkAddress targetaddress, IocpObjectRef core, SessionFactory factory, int32 maxSessionCount)
	:Service(ServiceType::Client, targetaddress, core, factory, maxSessionCount)
{
}

bool ClientService::Start()
{
	return true;
}

ServerService::ServerService(NetworkAddress address, IocpObjectRef core, SessionFactory factory, int32 maxSessionCount)
	:Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;
	_listener = make_shared<Listener>();
	if (_listener == nullptr)
		return false;

	ServerServiceRef service =static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;
	return true;
}

void ServerService::CloseService()
{
	//TODO
	Service::CloseService();
}