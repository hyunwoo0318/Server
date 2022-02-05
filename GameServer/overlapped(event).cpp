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


void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "ErrorCode : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE];
	int32 recvBytes = 0;
	int32 sendBytes = 0;
	WSAOVERLAPPED overlapped = {};
};


int main()
{
	//���� �ʱ�ȭ
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	//���ŷ(Blocking) = ���� �����尡 ���̻� �ڵ带 �������� �ʰ� block���·� ���������� �˷���
	//  -- ��������
	// accept -> ������ Ŭ�� ������
	// connect-> ���� ���ӿ� ����������
	// send, sendto -> ��û�� �����͸� �۽� ���ۿ� ����������
	// recv, recvfrom -> ���Ź��ۿ� ������ ������ �ְ�, �̸� �������� ���ۿ� ����������

	//����ŷ(Non-Blocking)

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	//Overlappped IO(�񵿱� + ����ŷ)
	//-overlapped �Լ��� �Ǵ�(WSARecv, WSASend)
	//-overlapped �Լ��� �����ߴ��� Ȯ�� ��
	// -> ���������� ����� �� ó��
	//-> ���������� ������ Ȯ���ؾ���.

	//�ѹ濡 �������� �ű���� ����
	//char sendBuffer[100];
	//WSABUF wsabuf[2];
	//wsabuf[0].buf = sendBuffer;
	//wsabuf[0].len = 100;

	//WSAOVERLAPPED wsaoverlapped;


	//1) �񵿱� ����� ����
	// 2)WSABUF �迭�� ���� �ּ� + ����  //Scatter-gather
	// 3)������ ���� ����Ʈ��
	// 4)�� �ɼ� 0
	// 5) WSAOVERLAPPED ����ü �ּҰ�
	// 6) ������� �Ϸ�Ǹ� os�� ȣ���� �ݹ��Լ� 
	//WSASend
	//WSARecv

	//Overlapped ��(�̺�Ʈ ���)
	//-�񵿱� ����� �����ϴ� ���� ���� + ���� �ޱ� ���� �̺�Ʈ ��ü ����
	// -�񵿱� ����� �Լ� ȣ��(1���� ���� �̺�Ʈ ��ü�� ���� �Ѱ���)
	// -�񵿱� �۾��� �ٷ� �Ϸ���� ������, WSA_IO_PENDING ���� �ڵ带 ����
	// �ü���� �̺�Ʈ ��ü�� SIGNALED ���·� ���� �Ϸ� ���¸� �˷���
	// -WSAWaitForMultipleEvents �Լ��� ȣ���ؼ� �̺�Ʈ ��ü�� signal�� �Ǻ���
	// -WsaGetOverlappedResult ȣ���ؼ� �񵿱� ����� ��� Ȯ�� �� ������ ó��
	// 

	//1)�񵿱� ����
	// 2)�Ѱ��� overlapped ����ü
	// 3) ���۵� ����Ʈ ��
	// 4) �񵿱� ����� �۾��� ���������� �������
	// false
	// 5)�񵿱� ����� �۾� ���� �ΰ�����, 
	//WsaGetOverlappedResult

	while (1)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);
		SOCKET clientSocket;
		while (1)
		{
			clientSocket = ::accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
				break;
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			//������� ���� �����ִ� ��Ȳ��
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session.overlapped.hEvent = wsaEvent;

		cout << " Client Connected!" << endl;

		while (1)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;

			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					//Pending
					::WSAWaitForMultipleEvents(1, &wsaEvent, true, WSA_INFINITE, false);
					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, false, &flags);
				}
				else
				{
					//TODO : �����ִ� ��Ȳ
				}
				
			}
			cout << "data Recv Len" << session.recvBuffer << endl;

			//���� ����
			::WSACleanup();
		}




	}
}