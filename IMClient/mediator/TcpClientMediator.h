#pragma once
#include "INetMediator.h"

class TcpClientMediator: public INetMediator
{
    Q_OBJECT

signals:
    //把接收到的数据都发送给kernel类处理
    void SIG_ReadyData(long lSendIP, char* buf, int nLen);

public:
	TcpClientMediator();
	~TcpClientMediator();
	//初始化网络
	bool OpenNet()override;
	//关闭网络
	void CloseNet()override;
	//发送数据
	bool SendData(long lSendIP, char* buf, int nLen)override;
	//接收数据
	void DealData(long lSendIP, char* buf, int nLen)override;
};

