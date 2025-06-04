#pragma once
#include "INet.h"
#include "INetMediator.h"

class TcpClientNet : public INet {
public:
	TcpClientNet(INetMediator* p_Mediator);
	~TcpClientNet();
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
	SOCKET m_sock;
	bool m_isStop;
	static unsigned __stdcall RecvThread(void* lpVoid);

};