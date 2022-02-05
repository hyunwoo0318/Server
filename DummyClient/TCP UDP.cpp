#include "pch.h"
#include <iostream>

#include<WinSock2.h>
#include<MSWSock.h>
#include<WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << "ErrorCode : " << errCode << endl;
//}
//
//int main()
//{
//	//윈속 초기화(ws2_32 라이브러리 초기화를 먼저 해야함)
//	// 관련 정보가 wsaData에 채워짐
//	//(버전, 설정값)
//	WSADATA wsaData;
//	//성공이면 0을 리턴
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	//socket(주소, type, 프로토콜)
//	//주소에는 IPv4, IPv6가 있는데 기본적으로 IPv4를 사용하고 이게 부족해서 만들어진게 IPv6이다.
//	// IPv4(AF_INET), ipv6(AF_INET6)
//	//TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)로 통신할지 정하는게 두번째 인자이다.
//	//세번째 인자에 0을 넣으면 컴파일러가 알아서 프로토콜을 정해준다.
//	//return : descriptor(socket을 return 해줘서 만들면 됨)
//
//	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (clientSocket == INVALID_SOCKET)
//	{
//		HandleError("socket");
//	}
//
//	//연결할 목적지는? (주소 + port) -> 아파트 + 호수
//	SOCKADDR_IN serverAddr; //IPV4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); 옛날방식
//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
//	serverAddr.sin_port = ::htons(7777); // 80:http 같이 특정 번호는 특정 용도로 독점되어있음
//
//	//host to network short
//	//Little_Endian vs Big_Endian
//	//0x12346578 4바이트 정수를 저장한다고 할때
//	//low [0x78][0x56][0x34][0x12] high >> little endian = 일반적인 컴퓨터나 모바일 장비
//	//low [0x12][0x34][0x56][0x78] high >> big endian = network
//
//	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		HandleError("connect");
//	}
//
//	//------------------
//	//연결성공 ! 이제부터 데이터 송수신이 가능해짐
//
//	cout << "Conneceted to Server!" << endl;
//
//	while (1)
//	{
//		char sendBuffer[100] = "HelloWorld!";
//
//		for (int32 i = 0; i < 10; i++)
//		{
//			int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
//			if (resultCode == SOCKET_ERROR)
//			{
//				HandleError("send");
//
//			}
//		}
//
//
//		cout << "send Data" << sizeof(sendBuffer) << endl;
//		this_thread::sleep_for(1s);
//
//		/*char recvBuffer[1000];
//
//		int32 recvLen = ::recv(clientSocket, sendBuffer, sizeof(recvBuffer), 0);
//		if (recvLen <= 0)
//		{
//			int32 errCode = ::WSAGetLastError();
//			cout << "Recieve error code" << errCode << endl;
//			return 0;
//
//		}*/
//	}
//
//	//소켓을 전부 사용했으므로 다 정리를 해달라고 부탁함(소켓 리소스 반환)
//	::closesocket(clientSocket);
//
//	//윈속 종료
//	::WSACleanup();
//}

//소켓 입출력 버퍼
//소켓을 만드는 순간에 입출력버퍼가 kernal level에서 생긴다.

//TCP의 특징) 데이터간의 경계선이 없어서 100바이트를 10개를 보내면
//받는 입장에서는 1000바이트로 인식한다.

//udp 실습

//int main()
//{
//	
//	WSADATA wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
//	if (clientSocket == INVALID_SOCKET)
//	{
//		HandleError("socket");
//		return 0;
//	}
//
//	
//	SOCKADDR_IN serverAddr; //IPV4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	
//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
//	serverAddr.sin_port = ::htons(7777); 
//
//	
//	cout << "Conneceted to Server!" << endl;
//
//	//connected UDP
//	::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
//
//
//	while (1)
//	{
//		char sendBuffer[100] = "HelloWorld!";
//
//		//나의 ip주소 + 포트 번호 설정
//
//		//Unconnected UDP
//		/*	int32 resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
//				(SOCKADDR*)&serverAddr, sizeof(serverAddr));*/
//			
//		//connected UDP
//			int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0); 
//			if (resultCode == SOCKET_ERROR)
//			{
//				HandleError("SendTo");
//				return 0;
//			}
//
//		cout << "send Data" << sizeof(sendBuffer) << endl;
//		this_thread::sleep_for(1ms);
//
//		SOCKADDR_IN recvAddr;
//		::memset(&recvAddr, 0, sizeof(recvAddr));
//		int32 addrLen = sizeof(recvAddr);
//
//		char recvBuffer[1000];
//
//		/*int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
//			(SOCKADDR*)&recvAddr, &addrLen);*/
//
//		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0
//			);
//		if (recvLen <= 0)
//		{
//			HandleError("recive");
//			return 0;
//		}
//		//cout << "recv data! data = " << recvBuffer << endl;
//		//cout << "recv data! Len = " << recvLen << endl;
//	}
//
//	//소켓을 전부 사용했으므로 다 정리를 해달라고 부탁함(소켓 리소스 반환)
//	::closesocket(clientSocket);
//
//	//윈속 종료
//	::WSACleanup();
//}