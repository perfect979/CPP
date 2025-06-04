#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TcpServerNet.h"
#include "TcpServerMediator.h"

TcpServerNet::TcpServerNet(INetMediator* p_Mediator):m_sock(INVALID_SOCKET), m_isStop(false)
{
	m_pMediator = p_Mediator;
}

TcpServerNet::~TcpServerNet()
{
	UninitNet();
}
//��ʼ�����磺���ؿ⡢�����׽��֡���IP��ַ������������һ���������ӵ��߳�
bool TcpServerNet::InitNet()
{
	//���ؿ�
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cout << "WSAStartup Faild:" << result << endl;
		return false;
	}
	//�����׽���
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		cout<< "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create Server socket Success!\n";
	}
	//��IP��ַ
	sockaddr_in serveraddr;
	//, clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_DEF_TCP_SERVER_PORT);
	serveraddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(m_sock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "Bind Error\n";
		return false;
	}
	else {
		cout << "bind success" << endl;
	}
	if (listen(m_sock, 5) == SOCKET_ERROR) {
		cout << "Listen Error\n";
		return false;
	}
	else {
		cout << "Listening...\n";
	}
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, NULL);
	m_listThreatHandle.emplace_back(handle);
	return true;
}
//�������ӵ��̺߳���
//ѭ���ȴ����տͻ�����
unsigned __stdcall TcpServerNet::AcceptThread(void* lpVoid) {
	TcpServerNet* pThis = (TcpServerNet*)lpVoid;
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (!pThis->m_isStop) {
		SOCKET sockClient = accept(pThis->m_sock, (sockaddr*)&addrClient, &addrClientSize);
		if (sockClient == INVALID_SOCKET) {
			cout << "Accecp Error\n";
		}
		else {
			cout << "IP: " << inet_ntoa(addrClient.sin_addr) << " Port: " << ntohs(addrClient.sin_port) << endl;
		}
		//����һ�����ӳɹ��Ŀͻ��˶�Ӧ�Ľ������ݵ��߳�
		unsigned int threadId = 0;
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &threadId);
		//������ͻ��˶�Ӧ��socket�ŵ�map�й���
		pThis->m_mapThreadIdToSocket[threadId] = sockClient;
		//���̵߳ľ������list�������
		if (handle) {
			pThis->m_listThreatHandle.emplace_back(handle);
		}

	}
	return 0;
}

unsigned __stdcall TcpServerNet::RecvThread(void* lpVoid) {

	TcpServerNet* pThis = (TcpServerNet*)lpVoid;
	pThis->RecvData();
	return 0;
}

void TcpServerNet::UninitNet()
{
	//1���˳��߳�
	m_isStop = true;
	for (auto ite = m_listThreatHandle.begin(); ite != m_listThreatHandle.end();) {
		if (*ite) {
			if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 100)) {
				TerminateThread(*ite, -1);
			}
			//2���رվ��
			CloseHandle(*ite);
			*ite = NULL;
		}
		ite = m_listThreatHandle.erase(ite);//�Ƴ���Ч�ڵ㣬����ֵ����һ����Ч�ڵ�
	}

	//3���ر��׽���
	if (m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
		if (ite->second && INVALID_SOCKET != ite->second) {
			closesocket(ite->second);
		}
		ite = m_mapThreadIdToSocket.erase(ite);
	}
	//4��ж�ؿ�
	WSACleanup();
}

bool TcpServerNet::SendData(long lSendIP, char* buf, int nLen)
{
	//1��У�����
	if (!buf || nLen <= 0) {
		cout << "TcpServerNet::SendData Parameter Error" << endl;
		return false;
	}
	//2���ȷ�����С
	if (send(lSendIP, (char*)&nLen, sizeof(int), 0) <= 0) {
		return false;
	}
	//3���ٷ�������
	if (send(lSendIP, buf, nLen, 0) <= 0) {
		return false;
	}
	return true;
}

void TcpServerNet::RecvData()
{
	//��Ϊ�̴߳��������У����ǰ�soket����map������Ҫһ��ʱ��
	//�������߳�������һ��
	Sleep(100);
	//1����ȡ��ǰ�̶߳�Ӧ��socket
	//��ȡ�߳�id
	unsigned int threadId = GetCurrentThreadId();
	SOCKET socket = m_mapThreadIdToSocket[threadId];
	//�ж�socket�ĺϷ���
	if (!socket || socket ==INVALID_SOCKET) {
		cout << "Socket ���Ϸ�\n";
		return;
	}
	int nRecvNum = 0;
	char recvBuf[4096] = "";
	int packSize = 0;
	int offset = 0;
	while (!m_isStop) {
		//1���Ƚ��հ��Ĵ�С
		nRecvNum = recv(socket, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {//������ճɹ�
			//ÿ�ν����궼newһ���µĿռ䣬�������յ����ݵ��µĿռ䣬��new�Ŀռ䴫�ݸ��н����ࡣ
			//��Ϊ���߳�����
			char* packBuf = new char[packSize];
			//�ٽ��հ�����
			while (packSize != 0) {
				nRecvNum = recv(socket, packBuf + offset, packSize, 0);
				offset += nRecvNum;
				packSize -= nRecvNum;
			}
			//�ѽ��յ������ݴ����н�����
			m_pMediator->DealData(socket, packBuf, offset);
			offset = 0;
		}
		else {
			cout << "TcpServerNet::RecvData Recv Error" << WSAGetLastError() << endl;
			break;
		}
	}
}
