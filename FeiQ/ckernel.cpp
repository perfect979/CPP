#include "ckernel.h"
#include <QDebug>
#include "UdpMediator.h"
#include <QMessageBox>
#include "INet.h"
#include "packDef.h"
#include <map>

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    //初始化主窗口
    m_pFeiqDlg = new FeiQDialog;
    m_pFeiqDlg->showNormal();

    //绑定好友列表类发送的信号
    QObject::connect(m_pFeiqDlg, SIGNAL(SIG_userClicked(QString)),
                     this, SLOT(SLOT_userClicked(QString))
                     );
    //绑定关闭窗口的信号和槽函数
    QObject::connect(m_pFeiqDlg, SIGNAL(SIG_closeDialog()),
                     this, SLOT(SLOT_closeDialog())
                     );
    //初始化中介者类
    m_pMediator = new UdpMediator;
    //打开网络
    if(!m_pMediator->OpenNet()){
        QMessageBox::about(m_pFeiqDlg, "提示","打开网络失败");
        exit(0);
    }
    /*
     * 哪里绑定：在接收信号的类里面发送信号的对象new出来以后
    */
    //绑定发送接收到所有数据的信号和槽
    QObject::connect(m_pMediator, SIGNAL(SIG_readyData(long,char*,int)),
                     this, SLOT(SLOT_readyData(long,char*,int))
                     );
    //发送一个广播,上线请求
    STRU_ONLINE rq;
    //获取当前主机的名字
    gethostname(rq.name, sizeof(rq.name));
    m_pMediator->SendData(INADDR_BROADCAST, (char*)&rq, sizeof(rq));


}

CKernel::~CKernel()
{
    if(m_pFeiqDlg){//回收资源
        m_pFeiqDlg->hide();
        delete m_pFeiqDlg;
        m_pFeiqDlg = nullptr;
    }
    if(m_pMediator){
        m_pMediator->CloseNet();
        delete m_pMediator;
        m_pMediator=nullptr;
    }

}

void CKernel::dealOnlineRq(long lSendIP, char *buf, int nLen)
{
    qDebug()<<__func__;
    // 1、拆包
    STRU_ONLINE* rq = (STRU_ONLINE*)buf;
    //2、添加该人到好友列表r
    m_pFeiqDlg->addFriend(INet::GetIPString(lSendIP).c_str(),rq->name);
    //3、创建与该人的聊天窗口
    chatdialog* chat = new chatdialog;
    //绑定发送聊天内容和ip的connect
    QObject::connect(chat, SIGNAL(SIG_sendMsg(QString,QString)),
                     this, SLOT(SLOT_sendMsg(QString,QString))
                     );

    chat->setInfo(INet::GetIPString(lSendIP).c_str());
    //4、把聊天窗口放到map<ip， 聊天窗口>
    m_mapIpToChatdlg[lSendIP] = chat;
    //5、判断是不是自己的上线请求，如果是自己的不回复
    std::set<long> setIp = INet::GetValidIpList();
    if(setIp.count(lSendIP) > 0){
        return;
    }
    //6、给这个人发送上线回复
    STRU_ONLINE rs;
    rs.nType = _DEF_PROTOCOL_UDP_ONLINE_RS;
    gethostname(rs.name, sizeof(rs.name));
    m_pMediator->SendData(lSendIP, (char*)&rs, sizeof (rs));
}

void CKernel::dealOnlineRs(long lSendIP, char *buf, int nLen)
{
    qDebug()<<__func__;
    //1、拆包
    STRU_ONLINE* rs = (STRU_ONLINE*)buf;
    //2、添加该人到好友列表
//    m_pFeiqDlg->addFriend(INet::GetIPString(lSendIP).c_str(), rs->name);
    //3、创建与该人的聊天窗口
    chatdialog* chat = new chatdialog;
    chat->setInfo(INet::GetIPString(lSendIP).c_str());
    QObject::connect(chat, SIGNAL(SIG_sendMsg(QString,QString)),
                     this, SLOT(SLOT_sendMsg(QString,QString))
                     );
    //4、把聊天窗口放到map<ip， 聊天窗口>
    m_mapIpToChatdlg[lSendIP] = chat;
}

void CKernel::dealOfflineRq(long lSendIP, char *buf, int nLen)
{
    qDebug()<<__func__;
    STRU_OFFLINE_RQ* rq = (STRU_OFFLINE_RQ*)buf;
    //1、将该好友从列表中删除
    m_pFeiqDlg->deleteFriend(INet::GetIPString(lSendIP).c_str());
    //2、回收与该好友的聊天窗口，并将无效节点从map中删除
    auto ite = m_mapIpToChatdlg.find(lSendIP);
    if(ite != m_mapIpToChatdlg.end()){
        chatdialog* chat = ite->second;
        chat->hide();
        delete chat;
        chat = nullptr;
    }
    m_mapIpToChatdlg.erase(ite);
}

void CKernel::dealChatRq(long lSendIP, char *buf, int nLen)
{
    qDebug()<<__func__;
    STRU_CHAT_RQ* rq = (STRU_CHAT_RQ*)buf;
    if(m_mapIpToChatdlg.count(lSendIP) > 0){
        chatdialog* chat = m_mapIpToChatdlg[lSendIP];
        chat->setContent(rq->content);
        chat->showNormal();
    }
}

void CKernel::SLOT_readyData(long lSendIP, char *buf, int nLen)
{
    qDebug()<<__func__;
    //取出协议头
    int nType = *(int*)buf;
    //根据协议头走到对应的流程
    switch (nType) {
    case _DEF_PROTOCOL_UDP_ONLINE_RQ:
        dealOnlineRq(lSendIP, buf, nLen);
        break;
    case _DEF_PROTOCOL_UDP_ONLINE_RS:
        dealOnlineRs(lSendIP, buf, nLen);
        break;
    case _DEF_PROTOCOL_UDP_OFFLINE_RQ:
        dealOfflineRq(lSendIP, buf, nLen);
        break;
    case _DEF_PROTOCOL_UDP_CHAT_RQ:
        dealChatRq(lSendIP, buf, nLen);
        break;
    }
}

void CKernel::SLOT_userClicked(QString ip)
{
    qDebug()<<__func__;
    //检查map中是否有与该人的聊天窗口，如果有，就显示窗口
    long lIp = inet_addr(ip.toStdString().c_str());
    if(m_mapIpToChatdlg.count(lIp) > 0){
        chatdialog* chat = m_mapIpToChatdlg[lIp];
        chat->showNormal();
    }
}

void CKernel::SLOT_sendMsg(QString content, QString ip)
{
    qDebug()<<__func__;
    qDebug() << "ip:" << ip << " content:" <<content;
    //打包成聊天请求，发送给对方
    STRU_CHAT_RQ rq;
    strcpy(rq.content, content.toStdString().c_str());
    m_pMediator->SendData(inet_addr(ip.toStdString().c_str()), (char*)&rq, sizeof(rq));
}

void CKernel::SLOT_closeDialog()
{
    // 1、发送下线请求
    STRU_OFFLINE_RQ rq;
    m_pMediator->SendData(INADDR_BROADCAST, (char*)&rq, sizeof(rq));
    // 2、回收资源
    if(m_pFeiqDlg){
        m_pFeiqDlg->hide();
        delete m_pFeiqDlg;
        m_pFeiqDlg = nullptr;
    }
    if(m_pMediator){
        m_pMediator->CloseNet();
        delete m_pMediator;
        m_pMediator = nullptr;
    }
    for(auto ite = m_mapIpToChatdlg.begin(); ite !=m_mapIpToChatdlg.end();){
        chatdialog* chat = ite->second;
        if(chat){
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        ite = m_mapIpToChatdlg.erase(ite);
    }

    // 3、退出进程
    exit(0);
}
