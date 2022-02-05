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
//	WSAOVERLAPPED overlapped = {};
//};
//
//void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
//{
//	cout << "Data Recv Len Callback = " << recvLen << endl;
//	//TODO:
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
//	//Overlappped IO(비동기 + 논블로킹)
//	//-overlapped 함수를 건다(WSARecv, WSASend)
//	//-overlapped 함수가 성공했는지 확인 후
//	// -> 성공했으면 결과를 얻어서 처리
//	//-> 실패했으면 사유를 확인해야함.
//
//	//한방에 여러개를 옮길수도 있음
//	//char sendBuffer[100];
//	//WSABUF wsabuf[2];
//	//wsabuf[0].buf = sendBuffer;
//	//wsabuf[0].len = 100;
//
//	//Overlapped 모델(Completion Routine 콜백 기반)
//	//-비동기 입출력 지원하는 소켓 생성
//	// -비동기 입출력 함수 호출(완료 루틴의 시작 주소를 넘겨준다)
//	// -비동기 작업은 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드를 남김
//	// -비동기입출력 함수 호출한 쓰레드를 -> Alterable Wait 상태로 만든다
//	// ex) WaitForSingleObjectEx, WaitForMultipleObjectEx, SleepEx, WSAWaitForMultipleEvents
//	// 비동기 입출력이 완료되면, 운영체제는 완료 루틴 호출
//	// -완료 루틴 호출이 모두 끝나면, 쓰레드는 Alterable Wait 상태에서 빠져나온다.
//
//	//1)오류 발생시 0이 아닌 값
//	// 2) 전송 바이트수
//	// 3)비동기 입출력 함수 호출 시 넘겨준 wsaoverlapped 주소값
//	//void CompletionRoutine()
//
//	//Select 모델
//	// ->장점) 윈도우/리눅스 공통.
//	// -단점) 성능 최하(매번 등록 비용), 64개의 제한
//	//WSAEventSelect 모델
//	// -장점) 비교적 뛰어난 성능
//	// 단점)64개의 제한
//	//Overlapped (이벤트 기반)
//	// 장점) 성능
//	// 단점) 64개의제한
//	//Overlapped (콜백 기반)
//	// 장점) 성능
//	// 단점) 모든 비동기 소켓 함수에서 사용 가능하지 않음, 빈번한 alertable wait로 인한 성능 저하
//	//IOCP
//
//	//Reactor Pattern(~뒤늦게, 논블로킹 소켓, 소켓 상태 확인 후 -> 뒤늦게 recv send 호출)
//	//Proactor Pattern(~미리, overlapped wsa~ )
//
//	while (1)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket;
//		while (1)
//		{
//			clientSocket = ::accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//				break;
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//				continue;
//
//			//여기까지 오면 문제있는 상황임
//			return 0;
//		}
//
//		Session session = Session{ clientSocket };
//		WSAEVENT wsaEvent = ::WSACreateEvent();
//
//
//		cout << " Client Connected!" << endl;
//
//		while (1)
//		{
//			WSABUF wsaBuf;
//			wsaBuf.buf = session.recvBuffer;
//			wsaBuf.len = BUFSIZE;
//
//			DWORD recvLen = 0;
//			DWORD flags = 0;
//
//			//마지막 인자가 콜백함수를 의미한다.
//			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
//			{
//				if (::WSAGetLastError() == WSA_IO_PENDING)
//				{
//					//Pending
//					//alertable wait
//
//					//마지막 인자를 true로 두면 alertable wait상태로 바꿈
//					//::WSAWaitForMultipleEvents(1, &wsaEvent, true, WSA_INFINITE, true);
//					::SleepEx(INFINITE, true);
//
//				}
//				else
//				{
//					//TODO : 문제있는 상황
//				}
//
//			}
//			else
//			{
//				cout << "data Recv Len" << recvLen << endl;
//			}
//
//
//
//		}
//
//
//
//	}
//
//	//윈속 종료
//	::WSACleanup();
//}