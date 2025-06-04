#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include <QMap>
#include "mychatdialog.h"
#include "INetMediator.h"
#include "chatdialog.h"
#include "packDef.h"
#include "logindialog.h"
#include "useritem.h"

//定义函数指针
class CKernel;
typedef void (CKernel::*pfun)(long, char*, int);

class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = nullptr);
    ~CKernel();
    //初始化协议数组
    void setProtocolMap();
    // utf-8(QString) => gb2312(char*) 
    //bgBuf是返回值
    static void utf8ToGb2312(char* bgBuf, int nLen, QString utf8); 
    // gb2312 => utf-8
    static QString gb2312ToUtf8(char* gbBuf);
    //处理上线回复
    void dealRegisterRs(long lSendIP, char* buf, int nLen);
    //处理登录回复
    void dealLoginRs(long lSendIP, char* buf, int nLen);
    //处理好友信息请求
    void dealFriendInfoRq(long lSendIP, char* buf, int nLen);
    //处理聊天回复
    void dealChatRs(long lSendIP, char* buf, int nLen);
    //处理聊天请求
    void dealChatRq(long lSendIP, char* buf, int nLen);
    //处理添加好友请求
    void dealAddFriendRq(long lSendIP, char* buf, int nLen);
    //处理添加好友回复
    void dealAddFriendRs(long lSendIP, char* buf, int nLen);
    //处理下线请求
    void dealOfflineRq(long lSendIP, char* buf, int nLen);
signals:

public slots:
    //处理所有接收到的数据
    void SLOT_ReadyData(long lSendIP, char* buf, int nLen);
    //处理注册信息
    void SLOT_registerCommit(QString tel, QString name, QString password);
    //处理登录信息
    void SLOT_loginCommit(QString tel, QString password);
    //处理显示与好友的聊天窗口信号
    void SLOT_userClicked(int id);
    //处理聊天内容和id
    void SLOT_sendMsg(QString content, int id);
    //处理添加好友的信号
    void SLOT_addFriend();
    //处理关闭登录和注册的信号
    void SLOT_closeLogindig();
    //处理关闭好友列表界面的信号
    void SLOT_closeMyChatdlg();
private:
    int m_id;
    QString m_name;
    MyChatDialog* m_pMainWnd;
    INetMediator* m_pMediator;
    LoginDialog* m_pLoginDlg;
    //定义歌数组，下标是协议头计算的，数组的内容数函数指针
    pfun m_netProtocolMap[_DEF_TCP_PROTOCOL_COUNT];
    //把useritem放到map中保存，key是id
    QMap<int, UserItem*> m_mapIdToUseritem;
    //把聊天窗口放到map中保存，key是id
    QMap<int, chatdialog*> m_mapIdToChatdig;

};

#endif // CKERNEL_H
