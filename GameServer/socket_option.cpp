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
//	//���� �ʱ�ȭ
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
//	//�ɼ��� �ؼ��ϰ� ó���� ��ü -> level
//	//���� �ڵ� -> sol_socket
//	//IPv4 -> IPPROTO_IP
//	//tcp �������� -> IPPROTO_TCP
//
//	//so_keepalive = �ֱ������� ���� ���� Ȯ�� ����(tcp only)
//	//������ �Ҹ��ҹ����� ������ ���´ٸ�?
//	//�ֱ����׷� tcp �������� ���� ���� Ȯ�� -> ������ ���� ����
//	bool enable = true;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
//
//	//so_linger : �����ϴ�
//	//�۽� ���ۿ� �ִ� �����͸� �������ΰ�? ���� ���ΰ�?
//	//onoff = 0�̸� closersocket()�� �ٷ� ����, �ƴϸ� linger�� ��ŭ ���(default 0)
//	//linger = ���ð�
//	LINGER linger;
//	linger.l_linger = 5;
//	linger.l_onoff = 1;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
//
//	//half-close
//	//sd_send : send�� ���´� sd_recieve, sd_both
//	//::shutdown(serverSocket, SD_SEND);
//
//	//sd_sndbuf = �۽� ������ ũ��
//	//sd_rcvbuf = ���� ������ ũ��
//
//	int32 sendBufferSize;
//	int32 optionLen = sizeof(sendBufferSize);
//	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen);
//	cout << "�۽� ���� ũ��:" << sendBufferSize << endl;
//
//	int32 rcvBufferSize;
//	int32 optionLen = sizeof(rcvBufferSize);
//	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBufferSize, &optionLen);
//	cout << "�۽� ���� ũ��:" << rcvBufferSize << endl;
//
//	//so_reuseaddr
//	//ip�ּ� �� ��Ʈ�� ������
//
//	bool enable = true;
//	::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)enable, sizeof(enable));
//
//	//IPPROTO_TCP
//	//TCP_NODELAY = nagle ���̱� �˰��� �۵� ����
//	//�����Ͱ� ����� ũ�� ������, �׷��������� �����Ͱ� ����� ���϶� ���� ���!
//	//���� : ���� ��Ŷ�� ���ʿ��ϰ� ���� �����Ǵ� ���� ����
//	//���� : ���� �ð� ����
//	{
//		bool enable = true;
//		::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)enable, sizeof(enable));
//	}
//
//
//	//���� ����
//	::WSACleanup();
////}
//

