#pragma once
#include<iostream>
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include "packDef.h"
#pragma comment(lib, "ws2_32.lib")  // ���ӵ�Windows Socket��
using namespace std;

using namespace std;
class INetMediator;
class INet {
public:
	INet() {};
	virtual ~INet() = default;
	//��ʼ������
	virtual bool InitNet() = 0;
	//�ر�����
	virtual void UninitNet() = 0;
	//��������
	virtual bool SendData(long lSendIP, char* buf, int nLen) = 0;
protected:
	//��������
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;
};
