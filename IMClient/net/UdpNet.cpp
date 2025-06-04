#include "UdpNet.h"
#include "INetMediator.h"
#include<process.h>

UdpNet::UdpNet(INetMediator* p_Mediator):m_handle(0), m_sock(INVALID_SOCKET), m_isStop(false) {
	m_pMediator = p_Mediator;
}

UdpNet::~UdpNet()
{
	UninitNet();
}
//��ʼ������
//���ؿ⡢�����׽��֡���ip��ַ������㲥Ȩ�ޣ����޹㲥��ַ����
//�������������̣߳����߳���һֱ�ȴ��������ݣ�
bool UdpNet::InitNet()
{
	//1�����ؿ�
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << std::endl;
		return false;
	}
	//2�������׽���
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_sock == INVALID_SOCKET) {
		std::cerr << "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create socket success\n";
	}
	//3����IP��ַ
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_DEF_UDP_SERVER_PORT);
	serveraddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(m_sock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Bind success\n";
	}
	//4������㲥Ȩ��
	int broadcast = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
		std::cerr << "setsockopt (SO_BROADCAST) failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "setsockopt success\n";
	}
	//5�������������ݵ��߳�
	//_beginthreadex��_endthreadexһ��ʹ��
	//_endthreadex���ڽ����߳�ʱ���Ȼ��տռ䣬��ExitThread
	m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);
	
	 
	return true;
}
// �������ݵ��̺߳���
unsigned __stdcall UdpNet::RecvThread(void* lpVoid) {

	UdpNet* pThis = (UdpNet*)lpVoid;
	pThis->RecvData();
	return 0;
}
// �ر�����
// �����̹߳�����ͨ����Ա������־λ�������վ���������ں˶��󣩡�
// �ر��׽��֡�ж�ؿ�
void UdpNet::UninitNet()
{
	//1�������̹߳���
	m_isStop = true;
	//2.���վ��
	if (m_handle) {
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 500)) {
			TerminateThread(m_handle, -1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}
	//3���ر��׽���
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//4��ж�ؿ�
	WSACleanup();
}

bool UdpNet::SendData(long lSendIP, char* buf, int nLen)
{
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_DEF_UDP_SERVER_PORT);
	sockAddr.sin_addr.S_un.S_addr = lSendIP;

	if (sendto(m_sock, buf, nLen, 0, (sockaddr*)&sockAddr, sizeof(sockAddr)) <= 0) {
		cout << "UdpNet: SendData error" << WSAGetLastError() << endl;
		return false;
	}
	return true;
}

void UdpNet::RecvData()
{
	int nRecvNum = 0;
	sockaddr_in sockAddr;
	int sockAddrSize = sizeof(sockAddr);
	char recvBuf[4096] = "";
	while (!m_isStop) {
		nRecvNum = recvfrom(m_sock, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&sockAddr, &sockAddrSize);
		
		if (nRecvNum > 0) {
			//ÿ�ν����궼newһ���µĿռ䣬�������յ����ݵ��µĿռ䣬��new�Ŀռ䴫�ݸ��н����ࡣ
			char* packBuf = new char[nRecvNum];
			memcpy(packBuf, recvBuf, nRecvNum);
			m_pMediator->DealData(sockAddr.sin_addr.S_un.S_addr, packBuf, nRecvNum);
		}
	}
}
