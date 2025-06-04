#pragma once
#include "INet.h"
#include "INetMediator.h"

class TcpClientNet : public INet {
public:
	TcpClientNet(INetMediator* p_Mediator);
	~TcpClientNet();
	//初始化网络
	bool InitNet() override;
	//关闭网络
	void UninitNet() override;
	//发送数据
	bool SendData(long lSendIP, char* buf, int nLen) override;
protected:
	//接收数据
	void RecvData() override;
	HANDLE m_handle;
	SOCKET m_sock;
	bool m_isStop;
	static unsigned __stdcall RecvThread(void* lpVoid);

};