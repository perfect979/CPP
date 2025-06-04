#pragma once
#include "INetMediator.h"

class UdpMediator : public INetMediator {
    Q_OBJECT
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
signals:
    //把接收到的数据给kernel
    void SIG_readyData(long lSendIP, char* buf, int nLen);
};
