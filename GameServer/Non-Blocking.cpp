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


//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << "ErrorCode : " << errCode << endl;
//}


//int main()
//{
//	//���� �ʱ�ȭ
//	WSADATA wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	//���ŷ(Blocking) = ���� �����尡 ���̻� �ڵ带 �������� �ʰ� block���·� ���������� �˷���
//	//  -- ��������
//	// accept -> ������ Ŭ�� ������
//	// connect-> ���� ���ӿ� ����������
//	// send, sendto -> ��û�� �����͸� �۽� ���ۿ� ����������
//	// recv, recvfrom -> ���Ź��ۿ� ������ ������ �ְ�, �̸� �������� ���ۿ� ����������
//
//	//����ŷ(Non-Blocking)
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//		return 0;
//
//	u_long on = 1;
//	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//		return 0;
//
//	SOCKADDR_IN serverAddr;
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//		return 0;
//
//	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//		return 0;
//
//	cout << "Accept" << endl;
//
//	SOCKADDR_IN clientAddr;
//	int32 addLen = sizeof(clientAddr);
//
//	//Accept
//	while (1)
//	{
//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addLen);
//		if (clientSocket == INVALID_SOCKET)
//		{
//			//���� ����߾�� �ߴµ�.. �ʰ� ����ŷ���� �϶��
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//				continue;
//
//			//error
//			break;
//		}
//
//		cout << "client Connected" << endl;
//
//		//Recieve
//		while (1)
//		{
//			char recvBuffer[1000];
//			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//			if (recvLen == SOCKET_ERROR)
//			{
//				//���� ����߾�� �ߴµ�.. �ʰ� ����ŷ���� �϶��
//				if (::WSAGetLastError() == WSAEWOULDBLOCK)
//					continue;
//				break;
//			}
//			else if (recvLen == 0)
//				break;
//
//			cout << "recv Data Len " << recvLen << endl;
//
//			//send
//			while (1)
//			{
//				if (::send(clientSocket, recvBuffer, recvLen, 0) == SOCKET_ERROR)
//				{
//					if (::WSAGetLastError() == WSAEWOULDBLOCK)
//						continue;
//					break;
//				}
//				cout << "send Data Len " << recvLen << endl;
//			}
//		}
//	}
//
//	//���� ����
//	::WSACleanup();
//}
