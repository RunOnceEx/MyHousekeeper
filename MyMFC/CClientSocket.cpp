#include "stdafx.h"
#include "CClientSocket.h"


CClientSocket::CClientSocket():
	m_socket(0)
{
}


CClientSocket::~CClientSocket()
{
}

bool CClientSocket::ConnectServer(CHAR* szIp, WORD port)
{
	//��ʼ��
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//�����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//���÷�������ַ
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(szIp);
	serverAddr.sin_port = htons(port);
	//���ӷ�����
	int nRet = connect(m_socket, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		closesocket(m_socket);
		m_socket = NULL;
		WSACleanup();
		return false;
	}
	return true;
}

TCHAR* CClientSocket::Recv()
{
	//������Ϣ
	MessageContent msg = { 0 };
	if (SOCKET_ERROR == recv(m_socket, (char*)&msg, sizeof(MessageContent), NULL))
	{
		printf("recv error");
		return nullptr;
	}

	wsprintf(m_bufRecv, _T("%S"), msg.szFoldPath);

	return m_bufRecv;
}

//�ر��׽���
bool CClientSocket::Close()
{
	WSACleanup();
	closesocket(m_socket);
	return true;
}