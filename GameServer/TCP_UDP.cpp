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

//int main()
//{
//	//���� �ʱ�ȭ
//	WSADATA wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "socket error code" << errCode << endl;
//		return 0;
//	}
//
//	//���� �ּҴ�? (�ּ� + port) -> ����Ʈ + ȣ��
//	SOCKADDR_IN serverAddr; //IPV4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // �ʰ� �˾Ƽ� ������
//	serverAddr.sin_port = ::htons(7777);
//
//	//�ȳ����� ���� ���� -> �Ĵ��� ��ǥ ��ȣ
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "Bind error code" << errCode << endl;
//		return 0;
//	}
//
//	//���� ���� ( , ��⿭�� �ִ��ѵ�)
//	if (::listen(listenSocket, 10) == SOCKET_ERROR)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "Listen error code" << errCode << endl;
//		return 0;
//	}
//
//	while (1)
//	{
//		SOCKADDR_IN clientAddr; //IPV4
//		::memset(&clientAddr, 0, sizeof(clientAddr));
//		int32 addLen = sizeof(clientAddr);
//
//		SOCKET clientsocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addLen);
//		if (clientsocket == INVALID_SOCKET)
//		{
//			int32 errCode = ::WSAGetLastError();
//			cout << "Listen error code" << errCode << endl;
//			return 0;
//		}
//
//		//�մ� ����
//		char ipAddress[16];
//		//������ ���ڿ��� �ٲ��ִ� �Լ�
//		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
//		cout << "client connected! IP=" << ipAddress << endl;
//
//		//TODO
//		while (1)
//		{
//			char recvBuffer[1000];
//
//			this_thread::sleep_for(1s);
//
//			int32 recvLen = ::recv(clientsocket, recvBuffer, sizeof(recvBuffer), 0);
//			if (recvLen <= 0)
//			{
//				int32 errCode = ::WSAGetLastError();
//				cout << "Recieve error code" << errCode << endl;
//				return 0;
//
//			}
//			cout << "recv data = " << recvBuffer << endl;
//			cout << "recv len = " << recvLen << endl;
//
//			/*int32 resultCode = ::send(clientsocket, recvBuffer, recvLen, 0);
//			if (resultCode == SOCKET_ERROR)
//			{
//				int32 errCode = ::WSAGetLastError();
//				cout << "Send error code" << errCode << endl;
//				return 0;
//
//			}*/
//		}
//	}
//
//	//���� ����
//	::WSACleanup();
//}
//
//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << "ErrorCode : " << errCode << endl;
//}

//UDP �ǽ�

//int main()
//{
//	WSADATA wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
//	if (serverSocket == INVALID_SOCKET)
//	{
//		HandleError("socket");
//		return 0;
//	}
//
//	SOCKADDR_IN serverAddr; //IPV4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); �������
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		HandleError("bind");
//		return 0;
//	}
//
//	while (1)
//	{
//		SOCKADDR_IN clientAddr;
//		::memset(&clientAddr, 0, sizeof(clientAddr));
//		int32 addrLen = sizeof(clientAddr);
//
//		this_thread::sleep_for(1s);
//
//		char recvBuffer[1000];
//
//		int32 recvLen = ::recvfrom(serverSocket,recvBuffer, sizeof(recvBuffer), 0,
//			(SOCKADDR*)&clientAddr, &addrLen);
//
//		if (recvLen <= 0)
//		{
//			HandleError("recvFrom");
//			return 0;
//		}
//		cout << "recieve success" << recvBuffer << "  " << recvLen << endl;
//
//		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0
//			, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
//
//		if (errorCode == SOCKET_ERROR)
//		{
//			HandleError("SendTo");
//			return 0;
//		}
//
//		cout << "send Data" << recvLen << endl;
//
//	}
//
//	::WSACleanup();
//}
