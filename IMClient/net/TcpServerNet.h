#pragma once
#include "INet.h"
#include <process.h>
#include <map>
#include <list>
class TcpServerNet : public INet {
public:
	TcpServerNet(INetMediator* p_Mediator);
	~TcpServerNet();
	//��ʼ������
	bool InitNet() override;
	//�ر�����
	void UninitNet() override;
	//��������
	bool SendData(long lSendIP, char* buf, int nLen) override;
protected:
	void RecvData();
	SOCKET m_sock;
	bool m_isStop;
	//�������ݵ��߳�
	static unsigned __stdcall RecvThread(void* lpVoid);
	//�������ӵ��߳�
	static unsigned __stdcall AcceptThread(void* lpVoid);
	//����һ��MAP��key���߳�id��value�ǿͻ��˶�Ӧ��socket
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
	//�������߳̾��������ɾ��
	list<HANDLE> m_listThreatHandle;
};