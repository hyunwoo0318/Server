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
//	//윈속 초기화
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
//	//옵션을 해석하고 처리할 주체 -> level
//	//소켓 코드 -> sol_socket
//	//IPv4 -> IPPROTO_IP
//	//tcp 프로토콜 -> IPPROTO_TCP
//
//	//so_keepalive = 주기적으로 연결 상태 확인 여부(tcp only)
//	//상대방이 소리소문없이 연결을 끊는다면?
//	//주기저그로 tcp 프로토콜 연결 상태 확인 -> 끊어진 연결 감지
//	bool enable = true;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
//
//	//so_linger : 지연하다
//	//송신 버퍼에 있는 데이터를 보낼것인가? 날릴 것인가?
//	//onoff = 0이면 closersocket()이 바로 리턴, 아니면 linger초 만큼 대기(default 0)
//	//linger = 대기시간
//	LINGER linger;
//	linger.l_linger = 5;
//	linger.l_onoff = 1;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
//
//	//half-close
//	//sd_send : send를 막는다 sd_recieve, sd_both
//	//::shutdown(serverSocket, SD_SEND);
//
//	//sd_sndbuf = 송신 버퍼의 크기
//	//sd_rcvbuf = 수신 버퍼의 크기
//
//	int32 sendBufferSize;
//	int32 optionLen = sizeof(sendBufferSize);
//	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen);
//	cout << "송신 버퍼 크기:" << sendBufferSize << endl;
//
//	int32 rcvBufferSize;
//	int32 optionLen = sizeof(rcvBufferSize);
//	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBufferSize, &optionLen);
//	cout << "송신 버퍼 크기:" << rcvBufferSize << endl;
//
//	//so_reuseaddr
//	//ip주소 및 포트를 재사용함
//
//	bool enable = true;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)enable, sizeof(enable));
//
//	//IPPROTO_TCP
//	//TCP_NODELAY = nagle 네이글 알고리즘 작동 여부
//	//데이터가 충분히 크면 보내고, 그렇지않으면 데이터가 충분히 쌓일때 까지 대기!
//	//장점 : 작은 패킷이 불필요하게 많이 생성되는 일을 방지
//	//단점 : 반응 시간 손해
//	{
//		bool enable = true;
//		::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)enable, sizeof(enable));
//	}
//
//
//	//윈속 종료
//	::WSACleanup();
////}
//

