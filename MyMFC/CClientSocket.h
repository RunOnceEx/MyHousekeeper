#pragma once
class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	//��ʼ���ͻ����׽��֣����������ӷ�����
	bool ConnectServer(CHAR* szIp, WORD port);
	//���𴴽��߳��������շ���������Ϣ
	TCHAR *Recv();
	bool Close();
public:
	SOCKET m_socket;
	TCHAR m_bufRecv[2048];
};

