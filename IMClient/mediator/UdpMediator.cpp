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
//��ʼ������

bool UdpMediator::OpenNet() {
	if (!m_pINet->InitNet()) {
		return false;
	}
	return true;
}
//�������ݵ��̺߳���


void UdpMediator::CloseNet() {
	m_pINet->UninitNet();

}
//��������
bool UdpMediator::SendData(long lSendIP, char* buf, int nLen) {
	if (!m_pINet->SendData(lSendIP, buf, nLen)) {
		return false;
	}
	return true;
}
//��������
void UdpMediator::DealData(long lSendIP, char* buf, int nLen) {
	//TODO: ���ݸ�kernel��
	std::cout << "UdpMediator::DealData : " << buf << std::endl;
}