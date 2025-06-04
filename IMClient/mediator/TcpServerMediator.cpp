#include "TcpServerMediator.h"
#include "TcpServerNet.h"

TcpServerMediator::TcpServerMediator()
{
	m_pINet = new TcpServerNet(this);
}

TcpServerMediator::~TcpServerMediator()
{
	if (m_pINet) {
		delete m_pINet;
		m_pINet = nullptr;
	}
}

bool TcpServerMediator::OpenNet()
{
	if (!m_pINet->InitNet()) {
		cout << "TcpServerMediator::OpenNet Fail" << endl;
		return false;
	}
	return true;
}

void TcpServerMediator::CloseNet()
{
	m_pINet->UninitNet();
}

bool TcpServerMediator::SendData(long lSendIP, char* buf, int nLen)
{
	if (!m_pINet->SendData(lSendIP, buf, nLen)) {
		cout << "TcpServerMediator::SendData Fail" << endl;
		return false;
	}
	return true;
}

void TcpServerMediator::DealData(long lSendIP, char* buf, int nLen){
	//TODO:�����ݴ���Kernel
	cout << "TcpServerMediator::DealData: "<< buf << endl;
	//�ٰ����ݴ��ظ��ͻ���
	SendData(lSendIP, buf, nLen);
}
