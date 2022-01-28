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

	//WSAEventSelect - (WSAEventSelect) 함수가 핵심이 되는
	//소켓과 관련된 네트워크 이벤트를 [이벤트 객체]를 통해 감지

	//이벤트 객체 관련 함수들
	//생성 : WSACreateEvent (수동 리셋 Manual-reset + Non-Signaled 상태 시작)
	//삭제 : WSACloseEvent
	//신호상태감지 : WSAWaitForMultipleEvents
	//구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents

	//소켓 <-> 이벤트 객체 연동 -- 1대1 대응해야함
	//WSAEventSelect(socket, event, networkEvents);
	//-관심있는 네트워크 이벤트
	//FD_ACCEPT 
	/*FD_READ : 접속한 클라가 있음 accept
		FD_WRITE : 데이터 수신 가능 recv, recvfrom
		FD_CLOSE : 데이터 송신 가능 send, sendto
		FD_CONNECT : 통신을 위한 연결 절차 완료
		FD_CLOSE : 상대가 접속 종료
		FD_OOB : */

	//주의사항
	//WSAEventSelect 함수를 호출하면, 해당 소켓은 자동으로 넌블로킹 모드로 전환됨
	//accept() 함수가 리턴하는 소켓은 listenSocket과 동일한 속성을 갖는다
	//-따라서 clientSocket은 FD_READ, FD_WRITE 등을 다시 등록 필요
	//드물게 WSAEWOULDBLOCK 오류가 뜰수 있으니 예외 처리 필요
	//이벤트 발생시 적절한 소켓 함수 호출해야함 아니면 다음 번에는 동일 네트워크 이벤트가 발생 X
	//ex) FD_READ 이벤트 떴으면 recv()호출해야함 만일 호출하지 않으면 FD_READ 두번 다시 x

	//1)count, event
	// 2) waitAll : 모두 기다림? 하나만 완료 되어도 OK?
	// 3)timeout : 타임아웃
	// 4) 지금은 false
	// return : 완료된 첫번째 인덱스
	//WSAWaitForMultipleEvents

	//1)socket
	// 2) eventObject : socket과 연동된 이벤트 객체 핸들을 넘겨주면, 이벤트 객체를 non-signaled
	// 3)networkEvent : 네트워크 이벤트/ 오류 정보가 저장
	//WSAEnumNetworkEvents

	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);

	WSAEVENT listenEvent = ::WSACreateEvent();
	wsaEvents.push_back(listenEvent);
	sessions.push_back(Session{ listenSocket });
	//관찰선언
	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
		return 0;

	while (1)
	{
		//결과물 확인
		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE , WSA_INFINITE,FALSE);
		if (index == WSA_WAIT_FAILED)
			return 0;
			
		index -= WSA_WAIT_EVENT_0;

		

		WSANETWORKEVENTS networkEvents;
		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
			continue;

		//Listener 소켓 체크
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			//error-check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);

			SOCKET clientSocket =::accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;

				WSAEVENT clientEvent = ::WSACreateEvent();
				wsaEvents.push_back(clientEvent);
				sessions.push_back(Session{ clientSocket });
				//관찰선언
				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_CLOSE | FD_CLOSE) == SOCKET_ERROR)
					return 0;

			}
		}

		//Client Session 소켓 체크
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			//error-check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;
		}
	}
	
	//윈속 종료
	::WSACleanup();
}




