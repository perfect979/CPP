#pragma once
#include "INet.h"
#include <process.h>
#include <map>
#include <list>
class TcpServerNet : public INet {
public:
	TcpServerNet(INetMediator* p_Mediator);
	~TcpServerNet();
	//初始化网络
	bool InitNet() override;
	//关闭网络
	void UninitNet() override;
	//发送数据
	bool SendData(long lSendIP, char* buf, int nLen) override;
protected:
	void RecvData();
	SOCKET m_sock;
	bool m_isStop;
	//接收数据的线程
	static unsigned __stdcall RecvThread(void* lpVoid);
	//接收连接的线程
	static unsigned __stdcall AcceptThread(void* lpVoid);
	//定义一个MAP，key是线程id，value是客户端对应的socket
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
	//链表储存线程句柄，方便删除
	list<HANDLE> m_listThreatHandle;
};