#pragma once
#include<iostream>
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include "packDef.h"
#pragma comment(lib, "ws2_32.lib")  // 链接到Windows Socket库
using namespace std;

using namespace std;
class INetMediator;
class INet {
public:
	INet() {};
	virtual ~INet() = default;
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
	virtual bool SendData(long lSendIP, char* buf, int nLen) = 0;
protected:
	//接收数据
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;
};
