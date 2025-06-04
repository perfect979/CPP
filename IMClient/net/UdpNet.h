#pragma once
#include "INet.h"
class UdpNet: public INet {
public:
	UdpNet(INetMediator* p_Mediator);
	 ~UdpNet();
	//��ʼ������
	bool InitNet() override;
	//�ر�����
	void UninitNet() override;
	//��������
	bool SendData(long lSendIP, char* buf, int nLen) override;
protected:
	//��������
	void RecvData() override;
	HANDLE m_handle;
	static unsigned __stdcall RecvThread(void* lpVoid);
	SOCKET m_sock;
	bool m_isStop;

};