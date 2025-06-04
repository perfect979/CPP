#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QTime>
chatdialog::chatdialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatdialog)
{
    ui->setupUi(this);
}

chatdialog::~chatdialog()
{
    delete ui;
}

void chatdialog::setInfo(QString name, int id)
{
    //保存成员变量
    m_name = name;
    m_id = id;
    //设置窗口title
    setWindowTitle(QString("与[%1]的聊天窗口").arg(name));
}

void chatdialog::setContent(QString content)
{
    ui->tb_chat->append(QString("[%1] %2\n").arg(m_name).arg(QTime::currentTime().toString("hh::mm:ss")));
    ui->tb_chat->append(content);
}

void chatdialog::setFriendOffline()
{
    ui->tb_chat->append(QString("[%1] %2\n").arg(m_name).arg(QTime::currentTime().toString("hh::mm:ss")));
    ui->tb_chat->append(QString("好友不在线"));
}

void chatdialog::on_pb_send_clicked()
{
    //1、取出用户输入的数据，校验
    QString contnet = ui->textEdit->toPlainText();
    if(contnet.isEmpty()){
        return;
    }
    //2、清空编辑窗口
    contnet = ui->textEdit->toHtml();
    ui->textEdit->clear();
    //3、显示输入内容到浏览窗口
    //显示格式：【我】时间 换行内容
    ui->tb_chat->append(QString("[我] %1\n").arg(QTime::currentTime().toString("hh::mm:ss")));
    ui->tb_chat->append(contnet);
    //4、发送聊天内容和id地址给kernel
    Q_EMIT SIG_sendMsg(contnet, m_id);
}

