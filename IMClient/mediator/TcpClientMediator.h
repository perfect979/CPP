#pragma once
#include "INetMediator.h"

class TcpClientMediator: public INetMediator
{
    Q_OBJECT

signals:
    //�ѽ��յ������ݶ����͸�kernel�ദ��
    void SIG_ReadyData(long lSendIP, char* buf, int nLen);

public:
	TcpClientMediator();
	~TcpClientMediator();
	//��ʼ������
	bool OpenNet()override;
	//�ر�����
	void CloseNet()override;
	//��������
	bool SendData(long lSendIP, char* buf, int nLen)override;
	//��������
	void DealData(long lSendIP, char* buf, int nLen)override;
};

