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

//加载库、创建套接字、连接服务端、创建接收数据的线程
bool TcpClientNet::InitNet()
{
	//加载库
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "WSAStartup failed: " << result << std::endl;
		return false;
	}
	//2、创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		std::cout << "socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		cout << "Create Client socket success\n";
	}
	//3、连接服务端
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
	//创建一个接收数据的线程
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
	//1、关闭套接字
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//2、卸载库
	WSACleanup();
	//3、结束线程工作(标志位制成flase，等待线程自己结束工作，如果一定时间之内不能结束工作，再强制杀死线程)
	m_isStop = true;
	//4.回收句柄
	if (m_handle) {
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 500)) {//一定时间之内不能结束工作
			TerminateThread(m_handle, -1);
		}
		CloseHandle(m_handle);
		m_handle = NULL;
	}
}

//
bool TcpClientNet::SendData(long lSendIP, char* buf, int nLen)
{
	//1、校验参数
	if (!buf || nLen <= 0) {
		cout << "TcpClientNet::SendData Parameter Error" << endl;
		return false;
	}
	//2、先发包大小
	if (send(m_sock, (char*)&nLen, sizeof(int), 0) <= 0) {
		return false;
	}
	//3、再发包内容
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
		//1、先接收包的大小
		nRecvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {//如果接收成功
			//每次接收完都new一个新的空间，拷贝接收的数据到新的空间，将new的空间传递给中介者类。
			//因为多线程问题
            char* packBuf = new char[packSize];
			//再接收包内容
            while (packSize) {
				nRecvNum = recv(m_sock, packBuf + offset, packSize, 0);
				offset += nRecvNum;
				packSize -= nRecvNum;
			}
			//把接收到的数据传给中介者类
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
