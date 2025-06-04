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
//初始化网络：加载库、创建套接字、绑定IP地址、监听、创建一个接收连接的线程
bool TcpServerNet::InitNet()
{
	//加载库
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cout << "WSAStartup Faild:" << result << endl;
		return false;
	}
	//创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		cout<< "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create Server socket Success!\n";
	}
	//绑定IP地址
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
//接受连接的线程函数
//循环等待接收客户连接
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
		//创建一个连接成功的客户端对应的接收数据的线程
		unsigned int threadId = 0;
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &threadId);
		//把这个客户端对应的socket放到map中管理
		pThis->m_mapThreadIdToSocket[threadId] = sockClient;
		//把线程的句柄放在list里面管理
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
	//1、退出线程
	m_isStop = true;
	for (auto ite = m_listThreatHandle.begin(); ite != m_listThreatHandle.end();) {
		if (*ite) {
			if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 100)) {
				TerminateThread(*ite, -1);
			}
			//2、关闭句柄
			CloseHandle(*ite);
			*ite = NULL;
		}
		ite = m_listThreatHandle.erase(ite);//移除无效节点，返回值是下一个有效节点
	}

	//3、关闭套接字
	if (m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
		if (ite->second && INVALID_SOCKET != ite->second) {
			closesocket(ite->second);
		}
		ite = m_mapThreadIdToSocket.erase(ite);
	}
	//4、卸载库
	WSACleanup();
}

bool TcpServerNet::SendData(long lSendIP, char* buf, int nLen)
{
	//1、校验参数
	if (!buf || nLen <= 0) {
		cout << "TcpServerNet::SendData Parameter Error" << endl;
		return false;
	}
	//2、先发包大小
	if (send(lSendIP, (char*)&nLen, sizeof(int), 0) <= 0) {
		return false;
	}
	//3、再发包内容
	if (send(lSendIP, buf, nLen, 0) <= 0) {
		return false;
	}
	return true;
}

void TcpServerNet::RecvData()
{
	//因为线程创建既运行，但是把soket存入map当中需要一定时间
	//所以让线程先休眠一会
	Sleep(100);
	//1、获取当前线程对应的socket
	//获取线程id
	unsigned int threadId = GetCurrentThreadId();
	SOCKET socket = m_mapThreadIdToSocket[threadId];
	//判断socket的合法性
	if (!socket || socket ==INVALID_SOCKET) {
		cout << "Socket 不合法\n";
		return;
	}
	int nRecvNum = 0;
	char recvBuf[4096] = "";
	int packSize = 0;
	int offset = 0;
	while (!m_isStop) {
		//1、先接收包的大小
		nRecvNum = recv(socket, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {//如果接收成功
			//每次接收完都new一个新的空间，拷贝接收的数据到新的空间，将new的空间传递给中介者类。
			//因为多线程问题
			char* packBuf = new char[packSize];
			//再接收包内容
			while (packSize != 0) {
				nRecvNum = recv(socket, packBuf + offset, packSize, 0);
				offset += nRecvNum;
				packSize -= nRecvNum;
			}
			//把接收到的数据传给中介者类
			m_pMediator->DealData(socket, packBuf, offset);
			offset = 0;
		}
		else {
			cout << "TcpServerNet::RecvData Recv Error" << WSAGetLastError() << endl;
			break;
		}
	}
}
