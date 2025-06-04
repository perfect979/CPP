#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <process.h>
#include "TcpClientNet.h"
#include "packDef.h"
#include "TcpClientMediator.h"

TcpClientNet::TcpClientNet(INetMediator* p_Mediator):m_handle(NULL), m_sock(INVALID_SOCKET), m_isStop(false)
{
	m_pMediator = p_Mediator;	
}

TcpClientNet::~TcpClientNet()
{
	UninitNet();
}

//���ؿ⡢�����׽��֡����ӷ���ˡ������������ݵ��߳�
bool TcpClientNet::InitNet()
{
	//���ؿ�
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "WSAStartup failed: " << result << std::endl;
		return false;
	}
	//2�������׽���
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		std::cout << "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create Client socket success\n";
	}
	//3�����ӷ����
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_DEF_TCP_SERVER_PORT);
	serveraddr.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
	if (connect(m_sock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
		std::cout << "connect failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		std::cout << "connect success\n";
	}
	//����һ���������ݵ��߳�
	m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);

	return true;
}

unsigned __stdcall TcpClientNet::RecvThread(void* lpVoid) {

	TcpClientNet* pThis = (TcpClientNet*)lpVoid;
	pThis->RecvData();
	return 0;
}
void TcpClientNet::UninitNet()
{
	//1���ر��׽���
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//2��ж�ؿ�
	WSACleanup();
	//3�������̹߳���(��־λ�Ƴ�flase���ȴ��߳��Լ��������������һ��ʱ��֮�ڲ��ܽ�����������ǿ��ɱ���߳�)
	m_isStop = true;
	//4.���վ��
	if (m_handle) {
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 500)) {//һ��ʱ��֮�ڲ��ܽ�������
			TerminateThread(m_handle, -1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}
}

//
bool TcpClientNet::SendData(long lSendIP, char* buf, int nLen)
{
	//1��У�����
	if (!buf || nLen <= 0) {
		cout << "TcpClientNet::SendData Parameter Error" << endl;
		return false;
	}
	//2���ȷ�����С
	if (send(m_sock, (char*)&nLen, sizeof(int), 0) <= 0) {
		return false;
	}
	//3���ٷ�������
	if (send(m_sock, buf, nLen, 0) <= 0) {
		return false;
	}
	return true;
}

void TcpClientNet::RecvData()
{
	int nRecvNum = 0;
	int packSize = 0;
	int offset = 0;
	while (!m_isStop) {
		//1���Ƚ��հ��Ĵ�С
		nRecvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {//������ճɹ�
			//ÿ�ν����궼newһ���µĿռ䣬�������յ����ݵ��µĿռ䣬��new�Ŀռ䴫�ݸ��н����ࡣ
			//��Ϊ���߳�����
            char* packBuf = new char[packSize];
			//�ٽ��հ�����
            while (packSize) {
				nRecvNum = recv(m_sock, packBuf + offset, packSize, 0);
				offset += nRecvNum;
				packSize -= nRecvNum;
			}
			//�ѽ��յ������ݴ����н�����
            m_pMediator->DealData(m_sock, packBuf, offset);
			offset = 0;
		}
        else if(WSAGetLastError() == 10053){
            break;
        }
		else {
			cout << "TcpClientNet::RecvData Recv Error" << WSAGetLastError()  << endl;
			break;
		}
	}
}
