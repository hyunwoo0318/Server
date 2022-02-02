﻿#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	this_thread::sleep_for(1s);

	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	cout << "1" << endl;

	// Connect
	while (true)
	{

		if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			HandleError("connect");
			// 원래 블록했어야 했는데... 너가 논블로킹으로 하라며?
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				cout << "2" << endl;
				continue;
			}

			// 이미 연결된 상태라면 break
			if (::WSAGetLastError() == WSAEISCONN)
			{
				cout << "3" << endl;
				break;
			}

			// Error
			break;
		}
	}


		cout << "Connected to Server!" << endl;

		char sendBuffer[100] = "Hello World";
		WSAEVENT wsaEvent = ::WSACreateEvent();
		OVERLAPPED overlapped = {};
		overlapped.hEvent = wsaEvent;

		//Send
		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = sendBuffer;
			wsaBuf.len = 100;

			DWORD sendLen = 0;
			DWORD flags = 0;
			if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					::WSAWaitForMultipleEvents(1, &wsaEvent, true, WSA_INFINITE, false);
					::WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, false, &flags);
				}
				else
				{
					//TODO : 문제 있는 상황
					break;
				}
			}
			cout << "data Send Len" << sendBuffer << endl;

			this_thread::sleep_for(1s);
		}
		





				
}