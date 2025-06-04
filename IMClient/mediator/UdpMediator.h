#pragma once
#include "INetMediator.h"

class UdpMediator : public INetMediator {
public:
	UdpMediator();
	~UdpMediator();
	//初始化网络
	bool OpenNet()override;
	//关闭网络
	void CloseNet()override;
	//发送数据
	bool SendData(long lSendIP, char* buf, int nLen)override;
	//接收数据
	void DealData(long lSendIP, char* buf, int nLen)override;
};