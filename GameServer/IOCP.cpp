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
//	//int32 sendBytes = 0;
//	
//};
//
//enum IO_TYPE
//{
//	READ,
//	WRITE,
//	ACCEPT,
//	CONNECT,
//};
//
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	int32 type = 0; //read, write, accept, connect
//};
//
//void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
//{
//	cout << "Data Recv Len Callback = " << recvLen << endl;
//	//TODO:
//}
//
//void WorkerThreadMain(HANDLE iocpHandle)
//{
//	while (true)
//	{
//		//��� �Ϸ�Ȱ� �ִ��� Ȯ����
//		DWORD bytesTransferred = 0;
//		Session* session = nullptr;
//		OverlappedEx* overlappedEx = nullptr;
//		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
//			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		if (ret == false || bytesTransferred == 0)
//		{
//			continue;
//		}
//
//		ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);
//
//		cout << "recv data IOCP" << bytesTransferred << endl;
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;		
//		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//}
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
//	/*u_long on = 1;
//	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//		return 0;*/
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
//	//Overlappped �� (Completion Routine �ݹ� ���)
//	//-�񵿱� ����� �Լ� �Ϸ�Ǹ�, �����帶�� �ִ� APC ť�� �ϰ��� ����
//	//-alertable wait���·� ���� apc ť ����(�ݹ� �Լ�)
//	//����) APCť�� �����帶�� �ִ�. Alertable Wait��ü�� ���� �δ㽺����.
//	//����) �̺�Ʈ ��� ����:�̺�Ʈ 1��1�����ؾ���.
//
//	//IOCP(Completion Port) ��
//	// -APC->Completion Port(�����帶�� �ִ°� �ƴϰ� 1��.�߾ӿ��� �����ϴ� APC ť����)
//	// Alertable Wait -> CP  ��� ó���� GetQueuedCompletionStatus
//	// ��Ƽ������� ������ ������ ����.
//
//	//CreateIoCompletionPort cp���� / ������ cp�� ���� ������� ������ ���
//	//GetQueuedCompletionStatus
//
//	//CP ���� 
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	vector<Session*> sessionManager;
//
//	//WorkerThreads
//	for (int32 i = 0; i < 5; i++)
//		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
//	
//	//Main Thread = Accpet ���
//	while (1)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
//		if (clientSocket == INVALID_SOCKET)
//			return 0;
//
//		Session* session = new Session();
//		session->socket = clientSocket;
//		sessionManager.push_back(session);
//
//		cout << " Client Connected!" << endl;
//
//		//������ cp�� ���
//		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, /*key*/(ULONG_PTR)session, 0);
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//
//		OverlappedEx* overlappedEx = new OverlappedEx();
//		overlappedEx->type = IO_TYPE::READ;
//
//
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//
//	}
//	GThreadManager->Join();
//	//���� ����
//	::WSACleanup();
//}