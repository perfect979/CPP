#pragma once
#include "INetMediator.h"

class UdpMediator : public INetMediator {
public:
	UdpMediator();
	~UdpMediator();
	//��ʼ������
	bool OpenNet()override;
	//�ر�����
	void CloseNet()override;
	//��������
	bool SendData(long lSendIP, char* buf, int nLen)override;
	//��������
	void DealData(long lSendIP, char* buf, int nLen)override;
};