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
//	//���� �ʱ�ȭ(ws2_32 ���̺귯�� �ʱ�ȭ�� ���� �ؾ���)
//	// ���� ������ wsaData�� ä����
//	//(����, ������)
//	WSADATA wsaData;
//	//�����̸� 0�� ����
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	//socket(�ּ�, type, ��������)
//	//�ּҿ��� IPv4, IPv6�� �ִµ� �⺻������ IPv4�� ����ϰ� �̰� �����ؼ� ��������� IPv6�̴�.
//	// IPv4(AF_INET), ipv6(AF_INET6)
//	//TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)�� ������� ���ϴ°� �ι�° �����̴�.
//	//����° ���ڿ� 0�� ������ �����Ϸ��� �˾Ƽ� ���������� �����ش�.
//	//return : descriptor(socket�� return ���༭ ����� ��)
//
//	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (clientSocket == INVALID_SOCKET)
//	{
//		HandleError("socket");
//	}
//
//	//������ ��������? (�ּ� + port) -> ����Ʈ + ȣ��
//	SOCKADDR_IN serverAddr; //IPV4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); �������
//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
//	serverAddr.sin_port = ::htons(7777); // 80:http ���� Ư�� ��ȣ�� Ư�� �뵵�� �����Ǿ�����
//
//	//host to network short
//	//Little_Endian vs Big_Endian
//	//0x12346578 4����Ʈ ������ �����Ѵٰ� �Ҷ�
//	//low [0x78][0x56][0x34][0x12] high >> little endian = �Ϲ����� ��ǻ�ͳ� ����� ���
//	//low [0x12][0x34][0x56][0x78] high >> big endian = network
//
//	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		HandleError("connect");
//	}
//
//	//------------------
//	//���Ἲ�� ! �������� ������ �ۼ����� ��������
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
//	//������ ���� ��������Ƿ� �� ������ �ش޶�� ��Ź��(���� ���ҽ� ��ȯ)
//	::closesocket(clientSocket);
//
//	//���� ����
//	::WSACleanup();
//}

//���� ����� ����
//������ ����� ������ ����¹��۰� kernal level���� �����.

//TCP�� Ư¡) �����Ͱ��� ��輱�� ��� 100����Ʈ�� 10���� ������
//�޴� ���忡���� 1000����Ʈ�� �ν��Ѵ�.

//udp �ǽ�

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
//		//���� ip�ּ� + ��Ʈ ��ȣ ����
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
//	//������ ���� ��������Ƿ� �� ������ �ش޶�� ��Ź��(���� ���ҽ� ��ȯ)
//	::closesocket(clientSocket);
//
//	//���� ����
//	::WSACleanup();
//}