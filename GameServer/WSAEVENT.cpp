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

//
//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << "ErrorCode : " << errCode << endl;
//}
//
//const int32 BUFSIZE = 1000;
//
//struct Session
//{
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUFSIZE];
//	int32 recvBytes = 0;
//	int32 sendBytes = 0;
//};
//
//
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
//	//WSAEventSelect - (WSAEventSelect) �Լ��� �ٽ��� �Ǵ�
//	//���ϰ� ���õ� ��Ʈ��ũ �̺�Ʈ�� [�̺�Ʈ ��ü]�� ���� ����
//
//	//�̺�Ʈ ��ü ���� �Լ���
//	//���� : WSACreateEvent (���� ���� Manual-reset + Non-Signaled ���� ����)
//	//���� : WSACloseEvent
//	//��ȣ���°��� : WSAWaitForMultipleEvents
//	//��ü���� ��Ʈ��ũ �̺�Ʈ �˾Ƴ��� : WSAEnumNetworkEvents
//
//	//���� <-> �̺�Ʈ ��ü ���� -- 1��1 �����ؾ���
//	//WSAEventSelect(socket, event, networkEvents);
//	//-�����ִ� ��Ʈ��ũ �̺�Ʈ
//	//FD_ACCEPT : Ŭ�� �����ϸ�
//	/*	FD_READ : ������ ���� ���� recv, recvfrom
//		FD_WRITE : ������ �۽� ���� send, sendto
//		
//		FD_CONNECT : ����� ���� ���� ���� �Ϸ�
//		FD_CLOSE : ��밡 ���� ����
//		FD_OOB : */
//
//		//���ǻ���
//		//WSAEventSelect �Լ��� ȣ���ϸ�, �ش� ������ �ڵ����� �ͺ��ŷ ���� ��ȯ��
//		//accept() �Լ��� �����ϴ� ������ listenSocket�� ������ �Ӽ��� ���´�
//		//���� clientSocket�� FD_READ, FD_WRITE ���� �ٽ� ��� �ʿ�
//		//�幰�� WSAEWOULDBLOCK ������ ��� ������ ���� ó�� �ʿ�
//		//�̺�Ʈ �߻��� ������ ���� �Լ� ȣ���ؾ��� �ƴϸ� ���� ������ ���� ��Ʈ��ũ �̺�Ʈ�� �߻� X
//		//ex) FD_READ �̺�Ʈ ������ recv()ȣ���ؾ��� ���� ȣ������ ������ FD_READ �ι� �ٽ� x
//
//		//1)count, event
//		// 2) waitAll : ��� ��ٸ�? �ϳ��� �Ϸ� �Ǿ OK?
//		// 3)timeout : Ÿ�Ӿƿ�
//		// 4) ������ false
//		// return : �Ϸ�� ù��° �ε���
//		//WSAWaitForMultipleEvents
//
//		//1)socket
//		// 2) eventObject : socket�� ������ �̺�Ʈ ��ü �ڵ��� �Ѱ��ָ�, �̺�Ʈ ��ü�� non-signaled
//		// 3)networkEvent : ��Ʈ��ũ �̺�Ʈ/ ���� ������ ����
//		//WSAEnumNetworkEvents
//
//	vector<WSAEVENT> wsaEvents;
//	vector<Session> sessions;
//	sessions.reserve(100);
//
//	WSAEVENT listenEvent = ::WSACreateEvent();
//	wsaEvents.push_back(listenEvent);
//	sessions.push_back(Session{ listenSocket });
//	//��������
//	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
//		return 0;
//
//	while (1)
//	{
//		//����� Ȯ��
//		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
//		if (index == WSA_WAIT_FAILED)
//			return 0;
//
//		index -= WSA_WAIT_EVENT_0;
//
//
//
//		WSANETWORKEVENTS networkEvents;
//		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
//			continue;
//
//		//Listener ���� üũ
//		if (networkEvents.lNetworkEvents & FD_ACCEPT)
//		{
//			//error-check
//			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
//				continue;
//
//			SOCKADDR_IN clientAddr;
//			int32 addrLen = sizeof(clientAddr);
//			SOCKET clientSocket = ::accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//			{
//				cout << "Client Connected" << endl;
//
//				WSAEVENT clientEvent = ::WSACreateEvent();
//				wsaEvents.push_back(clientEvent);
//				sessions.push_back(Session{ clientSocket });
//				//��������
//				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
//					return 0;
//
//			}
//		}
//
//		//Client Session ���� üũ
//		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
//		{
//			//error-check
//			if (networkEvents.iErrorCode[FD_READ] && networkEvents.iErrorCode[FD_READ_BIT] != 0)
//				continue;
//			//error-check
//			if (networkEvents.iErrorCode[FD_WRITE] && networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
//				continue;
//
//			Session& s = sessions[index];
//			if (s.recvBytes == 0)
//			{
//				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
//				if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
//				{
//					//TODO : ������ �����϶�
//					continue;
//				}
//				s.recvBytes = recvLen;
//				cout << "recv data " << recvLen << endl;
//
//			}
//
//			if (s.recvBytes > s.sendBytes)
//			{
//				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
//				if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
//				{
//					//TODO : ������ �����϶�
//					continue;
//				}
//				s.sendBytes += sendLen;
//				if (s.recvBytes == s.sendBytes)
//				{
//					s.recvBytes = 0;
//					s.sendBytes = 0;
//
//				}
//				cout << "send Data" << sendLen << endl;
//			}
//
//			//fd_closeó��
//			if (networkEvents.lNetworkEvents & FD_CLOSE)
//			{
//
//			}
//		}
//	}
//
//	//���� ����
//	::WSACleanup();
//}
