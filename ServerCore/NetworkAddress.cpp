#include "pch.h"
#include "NetworkAddress.h"

/*------------
	NetAddress
-------------*/

NetworkAddress::NetworkAddress(sockaddr_in sockAddr) : _sockAddr(sockAddr)
{
}

NetworkAddress::NetworkAddress(wstring ip, uint16 port)
{
	::memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr = Ip2Address(ip.c_str());
	_sockAddr.sin_port = ::htons(port);
}

wstring NetworkAddress::GetIPAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &_sockAddr.sin_addr, buffer, len32(buffer));
	return wstring();
}

IN_ADDR NetworkAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return IN_ADDR();
}
