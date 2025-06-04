#pragma once
#include "INet.h"
class UdpNet: public INet {
public:
	UdpNet(INetMediator* p_Mediator);
	 ~UdpNet();
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
	static unsigned __stdcall RecvThread(void* lpVoid);
	SOCKET m_sock;
	bool m_isStop;

};