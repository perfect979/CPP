#include "TcpClientMediator.h"
#include "TcpClientNet.h"

TcpClientMediator::TcpClientMediator()
{
	m_pINet = new TcpClientNet(this);
}

TcpClientMediator::~TcpClientMediator()
{
	if (m_pINet) {
		delete m_pINet;
		m_pINet = nullptr;
	}
}

bool TcpClientMediator::OpenNet()
{
	if (!m_pINet->InitNet()) {
		cout << "Client OpenNet fail" << endl;
		return false;
	}
	return true;
}

void TcpClientMediator::CloseNet()
{
	m_pINet->UninitNet();
}

bool TcpClientMediator::SendData(long lSendIP, char* buf, int nLen)
{
	if (!m_pINet->SendData(lSendIP, buf, nLen)) {
		cout << "Client SendData fail" << endl;
		return false;
	}
	return true;
}

void TcpClientMediator::DealData(long lSendIP, char* buf, int nLen)
{
    //通过信号传递给Kernel类
    Q_EMIT SIG_ReadyData(lSendIP, buf, nLen);
}
