#include "UdpNet.h"
#include "UdpMediator.h"
#include<process.h>

UdpNet::UdpNet(INetMediator* p_Mediator):m_handle(0), m_sock(INVALID_SOCKET), m_isStop(false) {
	m_pMediator = p_Mediator;
}

UdpNet::~UdpNet()
{
	UninitNet();
}
//初始化网络
//加载库、创建套接字、绑定ip地址、申请广播权限（有限广播地址）、
//创建接收数据线程（在线程里一直等待接收数据）
bool UdpNet::InitNet()
{
	//1、加载库
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << std::endl;
		return false;
	}
	//2、创建套接字
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_sock == INVALID_SOCKET) {
		std::cerr << "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create socket success\n";
	}
	//3、绑定IP地址
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
	//4、申请广播权限
	int broadcast = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
		std::cerr << "setsockopt (SO_BROADCAST) failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "setsockopt success\n";
	}
	//5、创建接收数据的线程
	//_beginthreadex和_endthreadex一起使用
	//_endthreadex会在结束线程时，先回收空间，在ExitThread
	m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);
	
	 
	return true;
}
// 接收数据的线程函数
unsigned __stdcall UdpNet::RecvThread(void* lpVoid) {

	UdpNet* pThis = (UdpNet*)lpVoid;
	pThis->RecvData();
	return 0;
}
// 关闭网络
// 结束线程工作（通过成员变量标志位）、回收句柄（回收内核对象）、
// 关闭套接字、卸载库
void UdpNet::UninitNet()
{
	//1、结束线程工作
	m_isStop = true;
	//2.回收句柄
	if (m_handle) {
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 500)) {
			TerminateThread(m_handle, -1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}
	//3、关闭套接字
    if (m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//4、卸载库
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
			//每次接收完都new一个新的空间，拷贝接收的数据到新的空间，将new的空间传递给中介者类。
			char* packBuf = new char[nRecvNum];
			memcpy(packBuf, recvBuf, nRecvNum);
			m_pMediator->DealData(sockAddr.sin_addr.S_un.S_addr, packBuf, nRecvNum);
		}
	}
}
