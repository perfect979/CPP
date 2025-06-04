#include "ckernel.h"
#include "TcpClientMediator.h"
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include "useritem.h"
#include <QIcon>
#include <QInputDialog>

//定义计算数组下标的宏替换公式
#define NetProtocolMap(a) m_netProtocolMap[a - _DEF_PROTOCOL_BASSE - 10]

CKernel::CKernel(QObject *parent) : QObject(parent), m_id(0)
{
    setProtocolMap();

    //创建登录和注册界面并显示
    m_pLoginDlg = new LoginDialog;
    m_pLoginDlg->showNormal();


    //绑定发送注册信息的信号和槽函数
    QObject::connect(m_pLoginDlg, SIGNAL(SIG_registerCommit(QString,QString,QString)),
                     this, SLOT(SLOT_registerCommit(QString,QString,QString))
                     );
    //绑定发送登录信息的信号和槽函数
    QObject::connect(m_pLoginDlg, SIGNAL(SIG_loginCommit(QString,QString)),
                     this, SLOT(SLOT_loginCommit(QString,QString))
                     );
    QObject::connect(m_pLoginDlg, SIGNAL(SIG_closeLogindig()),
                     this, SLOT(SLOT_closeLogindig())
                     );
    //创建好友列表窗口
    m_pMainWnd = new MyChatDialog;
//    m_pMainWnd->showNormal();


    connect(m_pMainWnd, SIGNAL(SIG_addFriend()),
            this, SLOT(SLOT_addFriend())
            );
    connect(m_pMainWnd, SIGNAL(SIG_closeMyChatdlg()),
            this, SLOT(SLOT_closeMyChatdlg())
            );

    //创建一个中介者类
    m_pMediator = new TcpClientMediator;

    //绑定处理所有数据的信号和槽函数
    QObject::connect(m_pMediator, SIGNAL(SIG_ReadyData(long,char*,int)),
                     this, SLOT(SLOT_ReadyData(long,char*,int))
                     );

    //打开网络
    if(!m_pMediator->OpenNet()){
        QMessageBox::about(m_pMainWnd, "提示", "打开网络失败");
        exit(0);
    }
    //测试代码、客户端给服务端发送“hello world”
//    STRU_LOGIN_RQ rq;
//    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));

}

CKernel::~CKernel()
{
    //回收 好友列表窗口
    if(m_pMainWnd){
        m_pMainWnd->hide();
        delete m_pMainWnd;
        m_pMainWnd = nullptr;
    }
    //回收中介者类
    if (m_pMediator){
        m_pMediator->CloseNet();
        delete m_pMediator;
        m_pMediator = nullptr;
    }
    if(m_pLoginDlg){
        m_pLoginDlg->hide();
        delete m_pLoginDlg;
        m_pLoginDlg = nullptr;
    }
}

void CKernel::setProtocolMap()
{
    //1、初始化数组
    memset(m_netProtocolMap, 0, sizeof(pfun)*_DEF_TCP_PROTOCOL_COUNT);
    //2、绑定协议头和处理函数（把处理函数放在对应的下标里）
    NetProtocolMap(_DEF_PROTOCOL_TCP_REGISTER_RS) = &CKernel::dealRegisterRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_LOOGIN_RS) = &CKernel::dealLoginRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_FRIEND_INFO) = &CKernel::dealFriendInfoRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_CHAT_RS) = &CKernel::dealChatRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_CHAT_RQ) = &CKernel::dealChatRq;
    NetProtocolMap(_DEF_PROTOCOL_ADD_FRIEND_RQ) = &CKernel::dealAddFriendRq;
    NetProtocolMap(_DEF_PROTOCOL_ADD_FRIEND_RS) = &CKernel::dealAddFriendRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_OFFLINE_RQ) = &CKernel::dealOfflineRq;
}

void CKernel::utf8ToGb2312(char *bgBuf, int nLen, QString utf8)
{
    QTextCodec* gb2312Code = QTextCodec::codecForName("gb2312");
    QByteArray ba = gb2312Code->fromUnicode(utf8);
    strcpy_s(bgBuf, nLen, ba.data());
}

QString CKernel::gb2312ToUtf8(char *gbBuf)
{
    QTextCodec* gb2312Code = QTextCodec::codecForName("gb2312");
    return gb2312Code->toUnicode(gbBuf);
}

void CKernel::dealRegisterRs(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_REGISTER_RS* rs = (STRU_REGISTER_RS*)buf;
    //2、根据注册结果显示提示信息
    switch(rs->result){
        case REGISTER_SUCCESS:QMessageBox::about(m_pLoginDlg, "提示", "注册成功");break;
        case NAME_IS_EXIST:QMessageBox::about(m_pLoginDlg, "提示", "注册失败，昵称已被使用");break;
        case TEL_IS_EXIST:QMessageBox::about(m_pLoginDlg, "提示", "注册失败，电话已被使用");break;

    }
}

void CKernel::dealLoginRs(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_LOGIN_RS* rs = (STRU_LOGIN_RS*)buf;
    //2、根据登录回复结果显示提示内容
    switch (rs->result) {

        //登录成功，保存当前登录用户的ID，隐藏登录窗口，显示好友列表窗口
        case LOGIN_SUCCESS:{m_id=rs->userId;m_pLoginDlg->hide();m_pMainWnd->showNormal();};break;
        case PASSWORD_ERROR:QMessageBox::about(m_pLoginDlg, "提示", "登录失败,密码错误");break;
        case USER_NOT_EXIT:QMessageBox::about(m_pLoginDlg, "提示", "登录失败,用户不存在");break;

    }
}

void CKernel::dealFriendInfoRq(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_FRIEND_INFO* info = (STRU_FRIEND_INFO*)buf;
    QString nameTemp = gb2312ToUtf8(info->name);
    QString feelingTemp = gb2312ToUtf8(info->feeling);
    //2、判断是不是自己
    if(info->userId == m_id){//是自己的信息，就设置到界面上
        //保存自己的名字
        m_name = nameTemp;
        m_pMainWnd->setUserInfo(nameTemp, feelingTemp, info->iconId);
        return;
    }
    //3、是好友的信息，线判断这个好友是否已经存在，如果不存在就new一个
    if(m_mapIdToUseritem.count(info->userId) == 0){
        //4、好友不存在，就new一个useritem
        UserItem* item = new UserItem;
        //5、设置useritem的属性控件
        item->setInfo(info->userId, info->state, info->iconId, nameTemp, feelingTemp);
        //6、把这个useritem添加到列表上
        m_pMainWnd->addFriend(item);
        //7、TPDO：创建一个与该好友的聊天窗口
        chatdialog* chat = new chatdialog;
        //8、TODO:设置聊天窗口
        chat->setInfo(nameTemp, info->userId);
        //9、绑定聊天窗口
        connect(chat, SIGNAL(SIG_sendMsg(QString,int)),
                this, SLOT(SLOT_sendMsg(QString,int))
                );
        //把聊天窗口保存到map
        m_mapIdToChatdig[info->userId] = chat;

        //绑定useritem显示聊天窗口的信号和槽函数
        QObject::connect(item, SIGNAL(SIG_userClicked(int)),
                         this, SLOT(SLOT_userClicked(int))
                         );

        //10、把useritem放到map中保存
        m_mapIdToUseritem[info->userId] = item;
    }
    else{//好友已经在列表中
        //11、取出已经存在的useritem
        UserItem* item = m_mapIdToUseritem[info->userId];
        qDebug() << "info->state"<<info->state;
        //12、重新设置控件的属性
        item->setInfo(info->userId, info->state, info->iconId, nameTemp, feelingTemp);
    }



}

void CKernel::dealChatRs(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_TCP_CHAT_RS* rs = (STRU_TCP_CHAT_RS*)buf;
    //2、找到聊天窗口
    if(m_mapIdToChatdig.count(rs->friendId) > 0){
        chatdialog* chat = m_mapIdToChatdig[rs->friendId];
        //3、设置好友不在线到聊天窗口上
        chat->setFriendOffline();
    }

}

void CKernel::dealChatRq(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_TCP_CHAT_RQ* rq = (STRU_TCP_CHAT_RQ*)buf;
    //2、找到聊天窗口
    if(m_mapIdToChatdig.count(rq->userId) > 0){
        chatdialog* chat = m_mapIdToChatdig[rq->userId];
        //3、设置聊天内容到窗口并显示
        chat->setContent(rq->content);
        chat->showNormal();
    }
}

void CKernel::dealAddFriendRq(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_ADD_FRIEND_RQ* rq= (STRU_ADD_FRIEND_RQ*)buf;
    STRU_ADD_FRIEND_RS rs;
    //2、弹出一个提示窗口，是否同意添加xxx为好友
    QString str = QString("用户【%1】请求添加你为好友，是否同意？").arg(rq->userName);
    if(QMessageBox::Yes == QMessageBox::question(m_pMainWnd, "添加好友", str)){//同意
        rs.result = ADD_SUCCESS;
    }else{//不同意
        rs.result = USER_REFUSE;
    }
    //3、返回给服务器添加结果
    rs.userId = rq->userId;
    rs.friendId = m_id;
    strcpy(rs.friendName, m_name.toStdString().c_str());
    m_pMediator->SendData(0, (char*)&rs, sizeof(rs));
}

void CKernel::dealAddFriendRs(long lSendIP, char *buf, int nLen)
{
    //1、拆包
    STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;
    //2、根据回复结果，显示提示信息
    switch (rs->result) {
        case ADD_SUCCESS:{QMessageBox::about(m_pMainWnd, "提示", QString("添加好友【%1】成功").arg(rs->friendName));}break;
        case NO_THIS_USER:{QMessageBox::about(m_pMainWnd, "提示", QString("添加好友【%1】失败，好友不存在").arg(rs->friendName));}break;
        case USER_OFFLINE:{QMessageBox::about(m_pMainWnd, "提示", QString("添加好友【%1】失败，好友不在线").arg(rs->friendName));}break;
        case USER_REFUSE:{QMessageBox::about(m_pMainWnd, "提示", QString("【%1】拒绝添加你为好友").arg(rs->friendName));}break;
    }
}

void CKernel::dealOfflineRq(long lSendIP, char *buf, int nLen)
{
    // 1、 拆包
    STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ*)buf;
    //2、找到下线好友的useritem，设置成离线状态
    if(m_mapIdToUseritem.count(rq->userId) > 0){
        UserItem* item = m_mapIdToUseritem[rq->userId];
        item->setUserOffline();
    }
}

void CKernel::SLOT_ReadyData(long lSendIP, char *buf, int nLen)
{
    //1、取出协议头
    int type = *(int*)buf;
//    2、根据协议头判断是哪个结构体，走对应的处理流程
//    2、1 判断协议头是否再有效范围内
    if ((type - _DEF_PROTOCOL_BASSE - 10) >= 0 &&
            (type - _DEF_PROTOCOL_BASSE - 10) < _DEF_TCP_PROTOCOL_COUNT){
        pfun pf = NetProtocolMap(type);
        if(pf){
            (this->*pf)(lSendIP, buf, nLen);
        }
        else{
            qDebug()<<"Type:" <<type;
        }
    }
    //处理完数据，空间回收
    delete []buf;
}

void CKernel::SLOT_registerCommit(QString tel, QString name, QString password)
{
    //1、打包
    STRU_REGISTER_RQ rq;
    strcpy(rq.tel, tel.toStdString().c_str());
    strcpy(rq.password, password.toStdString().c_str());
    utf8ToGb2312(rq.name, sizeof(rq.name), name);
//    strcpy(rq.name, name.toStdString().c_str());
    //2、通过中介者类把数据发给服务器
    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::SLOT_loginCommit(QString tel, QString password)
{
    //1、打包
    STRU_LOGIN_RQ rq;
    strcpy(rq.tel, tel.toStdString().c_str());
    strcpy(rq.password, password.toStdString().c_str());
    //2、通过中介者类把数据发给服务器
    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));

}

void CKernel::SLOT_userClicked(int id)
{
    //判断id对应的窗口是否存在
    if(m_mapIdToChatdig.count(id) > 0){
        //取出聊天窗口并显示
        chatdialog* chat = m_mapIdToChatdig[id];
        chat->showNormal();
    }
}

void CKernel::SLOT_sendMsg(QString content, int id)
{
    //1、打包
    STRU_TCP_CHAT_RQ rq;
    rq.userId = m_id;
    rq.friendId = id;
    strcpy(rq.content, content.toStdString().c_str());
    //2、发给服务端
    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::SLOT_addFriend()
{
    //1、弹出一个窗口，让用户输入昵称
    QString friendName = QInputDialog::getText(m_pMainWnd, "添加好友", "请输入好友的昵称");
    QString nameTemp = friendName;
    //2、校验用户输入的数据是否合法
    if (friendName.isEmpty() || nameTemp.remove(" ").isEmpty()){
        QMessageBox::about(m_pMainWnd, "提示", "输入为空或空字符串");
        return;
    }
    if(friendName.length() > 10 ){
        QMessageBox::about(m_pMainWnd, "提示", "称不能超过10");
        return;
    }
    //3、输入的昵称是不是自己
    if(friendName == m_name){
        QMessageBox::about(m_pMainWnd, "提示", "不能添加自己为好友");
        return;
    }
    //4、输入的昵称是不是已经是好友
    for(auto ite = m_mapIdToUseritem.begin(); ite != m_mapIdToUseritem.end();){
        UserItem* item = *ite;
        if(friendName == item->m_name){
            QMessageBox::about(m_pMainWnd, "提示", "不能添加现有好友");
            return;
        }
        ite++;
    }
    //5、以上都不满足，就给服务端发送添加好友的请求
    STRU_ADD_FRIEND_RQ rq;
    rq.userId = m_id;
    strcpy(rq.userName, m_name.toStdString().c_str());
    strcpy(rq.friendName, friendName.toStdString().c_str());

    utf8ToGb2312(rq.friendName, sizeof(rq.friendName), friendName);
    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::SLOT_closeLogindig()
{
    //1、回收资源
    if(m_pMainWnd){
        m_pMainWnd->hide();
        delete m_pMainWnd;
        m_pMainWnd = nullptr;
    }
    if(m_pLoginDlg){
        m_pLoginDlg->hide();
        delete m_pLoginDlg;
        m_pLoginDlg = nullptr;
    }
    if(m_pMediator){
        m_pMediator->CloseNet();
        delete m_pMediator;
        m_pMediator = nullptr;
    }
    for(auto ite = m_mapIdToChatdig.begin(); ite != m_mapIdToChatdig.end();){
        //回收节点中的指针
        chatdialog* chat = *ite;
        if(chat){
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        ite = m_mapIdToChatdig.erase(ite);
    }
    //2、结束进程
    exit(0);
}

void CKernel::SLOT_closeMyChatdlg()
{
    //1、给服务器发送下线请求
    STRU_TCP_OFFLINE_RQ rq;
    rq.userId = m_id;
    m_pMediator->SendData(0, (char*)&rq, sizeof(rq));
    //2、回收资源，结束进程
    SLOT_closeLogindig();
}
