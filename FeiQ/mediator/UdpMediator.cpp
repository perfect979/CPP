#include "UdpMediator.h"
#include "UdpNet.h"
UdpMediator::UdpMediator() {
	m_pINet = new UdpNet(this);
}
UdpMediator::~UdpMediator() {
	if (m_pINet) {
		m_pINet->UninitNet();
		delete m_pINet;
		m_pINet = nullptr;
	}
}
//初始化网络

bool UdpMediator::OpenNet() {
	if (!m_pINet->InitNet()) {
		return false;
	}
	return true;
}
//接收数据的线程函数


void UdpMediator::CloseNet() {
	m_pINet->UninitNet();

}
//发送数据
bool UdpMediator::SendData(long lSendIP, char* buf, int nLen) {
	if (!m_pINet->SendData(lSendIP, buf, nLen)) {
		return false;
	}
	return true;
}
//接收数据
void UdpMediator::DealData(long lSendIP, char* buf, int nLen) {
    //TODO: 通过信号将收到的信息传递给kernel类
    Q_EMIT SIG_readyData(lSendIP, buf, nLen);
//	std::cout << "UdpMediator::DealData : " << buf << std::endl;
}
