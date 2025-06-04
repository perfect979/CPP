#pragma once
#include<iostream>
#include<QObject>

using namespace std;

class INet; //������INet��
class INetMediator : public QObject{
    Q_OBJECT
public:
    INetMediator();
    virtual ~INetMediator();
	//��ʼ������
	virtual bool OpenNet() = 0;
	//�ر�����
	virtual void CloseNet() = 0;
	//��������
	virtual bool SendData(long lSendIP, char* buf, int nLen) = 0;
    //��������
	virtual void DealData(long lSendIP, char* buf, int nLen) = 0;
	INet* m_pINet;
};
