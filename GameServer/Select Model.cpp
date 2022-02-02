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
////Ŭ�� ������ �����ϸ� �� ������ session�� ���ؼ� �����Ѵ�.
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
//	// recv, recvfrom -> ���Ź��ۿ� ������ �����Ͱ� �ְ�, �̸� �������� ���ۿ� ����������
//
//	//����ŷ(Non-Blocking)
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//		return 0;
//
//	//FIONBIO�� socket�� I/O ���� �����Ҷ� ����ϴ� Ŀ�ǵ��̴�
//	//����° ���ڴ� u_long Ÿ������ 0�̸� blocking��� 0�� �ƴϸ� non-blocking����̴�.
//	//ioctlsocket�� ȣ�������ν� non-blocking���·� ��ȯ�Ѵ�.
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
//	//Select �� = (select) �Լ��� �ٽ��� �Ǵ� ��
//	//���� �Լ� ȣ���� ������ ������ �̸� �˼� �ִ�!
//	//������Ȳ)
//	//���Ź��ۿ� �����Ͱ� ���µ�, read�ϰų�
//	//�۽Ź��۰� ��á�µ�, write�Ѵٰų�
//	//-���ŷ ����: ������ �������� �ʾƼ� ���ŷ�Ǵ� ��Ȳ�� ����
//	//-����ŷ ���� : ������ �������� �ʾƼ� ���ʿ��ϰ� �ݺ� üũ�ϰԵǴ� ��Ȳ�� ����
//
//	//socket set
//	//1)�б�[] ����[] ����(OOB)[] ���� ��� ���
//	//OutOfBand�� Send() ������ ���� MSG_OOB�� ������ Ư���� �������ε� �޴��ʿ��� ������� ����������
//	//2)select(readSet, writeSet, exceptSet) -> ���� ����
//	//3)��� �ϳ��� ������ �غ�Ǹ� ���� -> �����ڴ� �˾Ƽ� ���ŵ�
//	//4) ���� ���� üũ�ؼ� ����
//
//	//fd_set set;
//	//FD_ZERO : ���� ex) FD_ZERO(set);
//	//FD_SET : ���� s�� �ִ´� FD_SET(s, &set)
//	//FD_CLR : ���� s�� �����Ѵ�. FD_CLR(s, &set)
//	//FD_ISSET : ���� s�� set�� ��������� 0�� �ƴѰ��� �����Ѵ�
//
//	vector<Session> sessions;
//	sessions.reserve(100);
//
//	fd_set reads;
//	fd_set writes;
//
//	while (1)
//	{
//		//���� �� �ʱ�ȭ
//		FD_ZERO(&reads);
//		FD_ZERO(&writes);
//
//		//listenSocket ���
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
//		//[�ɼ�] ������ timeout ���� ���� ����
//	/*	timeval Timeval;
//		Timeval.tv_sec = 1;
//		Timeval.tv_usec = 3;*/
//		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
//		if (retVal == SOCKET_ERROR)
//			break;
//
//		//Listener ���� üũ
//		if (FD_ISSET(listenSocket, &reads))
//		{
//			SOCKADDR_IN clientAddr;
//			int32 addrLen = sizeof(clientAddr);
//			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//			//select�� �����ڴ� �����ϱ⶧���� �����ϴ°��� client�� ���������� �˷��ִ°��̴�.
//			//�׷��� ������ ���鼭 �Ź����� �ʱ�ȭ�� ����� �������.
//			if (clientSocket != INVALID_SOCKET)
//			{
//				cout << " Client Connected" << endl;
//				sessions.push_back(Session{clientSocket});
//			}
//		}
//
//		//������ ���� üũ
//		for (Session& s : sessions)
//		{
//			//read
//			if (FD_ISSET(s.socket, &reads))
//			{
//				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
//				if (recvLen <= 0)
//				{
//					//TODO : session ����
//					continue;
//				}
//				s.recvBytes = recvLen;
//			}
//
//			//write
//			if (FD_ISSET(s.socket, &writes))
//			{
//				//���ŷ ��� -> ��� �����͸� ����
//				//����ŷ��� -> �Ϻθ� ����������(���� ���� ������ ��Ȳ������)
//				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
//				if (sendLen == SOCKET_ERROR)
//				{
//					//TODO : session ����
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
//	//���� ����
//	::WSACleanup();
//}
