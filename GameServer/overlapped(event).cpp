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
	//윈속 초기화
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	//블로킹(Blocking) = 현재 쓰레드가 더이상 코드를 진행하지 않고 block상태로 멈춰있음을 알려줌
	//  -- 성공조건
	// accept -> 접속한 클라가 있을때
	// connect-> 서버 접속에 성공했을때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을때
	// recv, recvfrom -> 수신버퍼에 도착한 데이턱 있고, 이를 유저레벨 버퍼에 복사했을때

	//논블로킹(Non-Blocking)

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

	//Overlappped IO(비동기 + 논블로킹)
	//-overlapped 함수를 건다(WSARecv, WSASend)
	//-overlapped 함수가 성공했는지 확인 후
	// -> 성공했으면 결과를 얻어서 처리
	//-> 실패했으면 사유를 확인해야함.

	//한방에 여러개를 옮길수도 있음
	//char sendBuffer[100];
	//WSABUF wsabuf[2];
	//wsabuf[0].buf = sendBuffer;
	//wsabuf[0].len = 100;

	//WSAOVERLAPPED wsaoverlapped;


	//1) 비동기 입출력 소켓
	// 2)WSABUF 배열의 시작 주소 + 개수  //Scatter-gather
	// 3)보내고 받은 바이트수
	// 4)상세 옵션 0
	// 5) WSAOVERLAPPED 구조체 주소값
	// 6) 입출력이 완료되면 os가 호출할 콜백함수 
	//WSASend
	//WSARecv

	//Overlapped 모델(이벤트 기반)
	//-비동기 입출력 지원하는 소켓 생성 + 통지 받기 위한 이벤트 객체 생성
	// -비동기 입출력 함수 호출(1에서 만든 이벤트 객체를 같이 넘겨줌)
	// -비동기 작업은 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드를 남김
	// 운영체제는 이벤트 객체를 SIGNALED 상태로 만들어서 완료 상태를 알려줌
	// -WSAWaitForMultipleEvents 함수를 호출해서 이벤트 객체의 signal을 판별함
	// -WsaGetOverlappedResult 호출해서 비동기 입출력 결과 확인 및 데이터 처리
	// 

	//1)비동기 소켓
	// 2)넘겨준 overlapped 구조체
	// 3) 전송된 바이트 수
	// 4) 비동기 입출력 작업이 끝날때까지 대기할지
	// false
	// 5)비동기 입출력 작업 관련 부가정보, 
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

			//여기까지 오면 문제있는 상황임
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
					//TODO : 문제있는 상황
				}
				
			}
			cout << "data Recv Len" << session.recvBuffer << endl;

			//윈속 종료
			::WSACleanup();
		}




	}
}