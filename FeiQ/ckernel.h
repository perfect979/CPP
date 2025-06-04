#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include "feiqdialog.h"
#include "INetMediator.h"
#include <map>
#include "chatdialog.h"

class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = nullptr);
    ~CKernel();
    //处理上线请求
    void dealOnlineRq(long lSendIP, char* buf, int nLen);
    //处理上线回复
    void dealOnlineRs(long lSendIP, char* buf, int nLen);
    //处理下线请求
    void dealOfflineRq(long lSendIP, char* buf, int nLen);
    //处理聊天请求
    void dealChatRq(long lSendIP, char* buf, int nLen);
signals:

public slots:
    //接收中介者类发送的处理所有数据的信号
    void SLOT_readyData(long lSendIP, char* buf, int nLen);
    //接收好友列表类发送的双击的好友ip地址
    void SLOT_userClicked(QString ip);
    //处理聊天内容和ip
    void SLOT_sendMsg(QString content, QString ip);
    //处理关闭窗口的信号
    void SLOT_closeDialog();
private:
    FeiQDialog*     m_pFeiqDlg;
    INetMediator*   m_pMediator;
    std::map<long, chatdialog*> m_mapIpToChatdlg;
};

#endif // CKERNEL_H
