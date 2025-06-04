#pragma once
#include<iostream>
#include<QObject>
using namespace std;

class INet; //声明有INet类
class INetMediator : public QObject{
    Q_OBJECT
public:
    INetMediator();
	virtual ~INetMediator() = default;
	//初始化网络
	virtual bool OpenNet() = 0;
	//关闭网络
	virtual void CloseNet() = 0;
	//发送数据
	virtual bool SendData(long lSendIP, char* buf, int nLen) = 0;
	//接收数据
	virtual void DealData(long lSendIP, char* buf, int nLen) = 0;
	INet* m_pINet;
};
