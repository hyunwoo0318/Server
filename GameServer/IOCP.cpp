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
//		//계속 완료된게 있는지 확인함
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
//	//윈속 초기화
//	WSADATA wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	//블로킹(Blocking) = 현재 쓰레드가 더이상 코드를 진행하지 않고 block상태로 멈춰있음을 알려줌
//	//  -- 성공조건
//	// accept -> 접속한 클라가 있을때
//	// connect-> 서버 접속에 성공했을때
//	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을때
//	// recv, recvfrom -> 수신버퍼에 도착한 데이턱 있고, 이를 유저레벨 버퍼에 복사했을때
//
//	//논블로킹(Non-Blocking)
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
//	//Overlappped 모델 (Completion Routine 콜백 기반)
//	//-비동기 입출력 함수 완료되면, 쓰레드마다 있는 APC 큐에 일감이 쌓임
//	//-alertable wait상태로 들어가서 apc 큐 비우기(콜백 함수)
//	//단점) APC큐가 쓰레드마다 있다. Alertable Wait자체도 조금 부담스러움.
//	//단점) 이벤트 방식 소켓:이벤트 1대1대응해야함.
//
//	//IOCP(Completion Port) 모델
//	// -APC->Completion Port(쓰레드마다 있는건 아니고 1개.중앙에서 관리하는 APC 큐같음)
//	// Alertable Wait -> CP  결과 처리를 GetQueuedCompletionStatus
//	// 멀티쓰레드랑 궁합이 굉장히 좋다.
//
//	//CreateIoCompletionPort cp생성 / 소켓을 cp에 관찰 대상으로 넣을때 사용
//	//GetQueuedCompletionStatus
//
//	//CP 생성 
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	vector<Session*> sessionManager;
//
//	//WorkerThreads
//	for (int32 i = 0; i < 5; i++)
//		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
//	
//	//Main Thread = Accpet 담당
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
//		//소켓을 cp에 등록
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
//	//윈속 종료
//	::WSACleanup();
//}