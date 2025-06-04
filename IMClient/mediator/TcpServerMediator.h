#pragma once
#include "INetMediator.h"
class TcpServerMediator : public INetMediator
{
public:
	TcpServerMediator();
	~TcpServerMediator();
	//��ʼ������
	bool OpenNet()override;
	//�ر�����
	void CloseNet()override;
	//��������
	bool SendData(long lSendIP, char* buf, int nLen)override;
	//��������
	void DealData(long lSendIP, char* buf, int nLen)override;
};

