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
//
//const int32 BUFSIZE = 1000;
//
////클라가 서버에 접속하면 그 정보를 session을 통해서 관리한다.
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
//	// recv, recvfrom -> 수신버퍼에 도착한 데이터가 있고, 이를 유저레벨 버퍼에 복사했을때
//
//	//논블로킹(Non-Blocking)
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//		return 0;
//
//	//FIONBIO는 socket을 I/O 모드로 설정할때 사용하는 커맨드이다
//	//세번째 인자는 u_long 타입으로 0이면 blocking모드 0이 아니면 non-blocking모드이다.
//	//ioctlsocket을 호출함으로써 non-blocking상태로 변환한다.
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
//	//Select 모델 = (select) 함수가 핵심이 되는 모델
//	//소켓 함수 호출이 성공할 시점을 미리 알수 있다!
//	//문제상황)
//	//수신버퍼에 데이터가 없는데, read하거나
//	//송신버퍼가 꽉찼는데, write한다거나
//	//-블로킹 소켓: 조건이 만족되지 않아서 블로킹되는 상황을 예방
//	//-논블로킹 소켓 : 조건이 만족되지 않아서 불필요하게 반복 체크하게되는 상황을 예방
//
//	//socket set
//	//1)읽기[] 쓰기[] 예외(OOB)[] 관찰 대상 등록
//	//OutOfBand는 Send() 마지막 인자 MSG_OOB로 보내는 특별한 데이터인데 받는쪽에도 맞춰줘야 받을수있음
//	//2)select(readSet, writeSet, exceptSet) -> 관찰 시작
//	//3)적어도 하나의 소켓이 준비되면 리턴 -> 낙오자는 알아서 제거됨
//	//4) 남은 소켓 체크해서 진행
//
//	//fd_set set;
//	//FD_ZERO : 비운다 ex) FD_ZERO(set);
//	//FD_SET : 소켓 s를 넣는다 FD_SET(s, &set)
//	//FD_CLR : 소켓 s를 제거한다. FD_CLR(s, &set)
//	//FD_ISSET : 소켓 s가 set에 들어있으면 0이 아닌값을 리턴한다
//
//	vector<Session> sessions;
//	sessions.reserve(100);
//
//	fd_set reads;
//	fd_set writes;
//
//	while (1)
//	{
//		//소켓 셋 초기화
//		FD_ZERO(&reads);
//		FD_ZERO(&writes);
//
//		//listenSocket 등록
//		FD_SET(listenSocket, &reads);
//
//		for (Session& s : sessions)
//		{
//			if (s.recvBytes <= s.sendBytes)
//				FD_SET(s.socket, &reads);
//			else
//				FD_SET(s.socket, &writes);
//		}
//
//		//[옵션] 마지막 timeout 인자 설정 가능
//	/*	timeval Timeval;
//		Timeval.tv_sec = 1;
//		Timeval.tv_usec = 3;*/
//		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
//		if (retVal == SOCKET_ERROR)
//			break;
//
//		//Listener 소켓 체크
//		if (FD_ISSET(listenSocket, &reads))
//		{
//			SOCKADDR_IN clientAddr;
//			int32 addrLen = sizeof(clientAddr);
//			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//			//select는 낙오자는 제거하기때문에 존재하는경우는 client가 접속했음을 알려주는것이다.
//			//그래서 루프를 돌면서 매번마다 초기화와 등록을 해줘야함.
//			if (clientSocket != INVALID_SOCKET)
//			{
//				cout << " Client Connected" << endl;
//				sessions.push_back(Session{clientSocket});
//			}
//		}
//
//		//나머지 소켓 체크
//		for (Session& s : sessions)
//		{
//			//read
//			if (FD_ISSET(s.socket, &reads))
//			{
//				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
//				if (recvLen <= 0)
//				{
//					//TODO : session 제거
//					continue;
//				}
//				s.recvBytes = recvLen;
//			}
//
//			//write
//			if (FD_ISSET(s.socket, &writes))
//			{
//				//블로킹 모드 -> 모든 데이터를 보냄
//				//논블로킹모드 -> 일부만 보낼수있음(상대방 수신 버퍼의 상황에따라)
//				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
//				if (sendLen == SOCKET_ERROR)
//				{
//					//TODO : session 제거
//					continue;
//				}
//
//				s.sendBytes += sendLen;
//				if (s.recvBytes == s.sendBytes)
//				{
//					s.recvBytes = 0;
//					s.sendBytes = 0;
//				}
//			}
//		}
//	}
//
//	//윈속 종료
//	::WSACleanup();
//}
