 #include "useritem.h"
#include "ui_useritem.h"
#include "packDef.h"
#include  "QIcon"
#include <QBitmap>
#include <QMessageBox>
UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem)
{
    ui->setupUi(this);
}

UserItem::~UserItem()
{
    delete ui;
}

void UserItem::setInfo(int id, int state, int iconId, QString name, QString feeling)
{
    //保存用户信息
    m_id = id;
    m_state = state;
    m_iconId = iconId;
    m_name = name;
    m_feeling = feeling;

    //设置控件属性
    ui->lb_namel->setText(m_name);
    ui->lb_feeling->setText(m_feeling);
    //2.2、设置头像
    //拼接头像文件路径
    QString iconPath = QString(":/icon/%1.png").arg(m_iconId);
    //判断用户状态，是否在线
    if(state == STATUS_ONLINE){//在线、亮显图标
        ui->pb_icon->setIcon(QIcon(iconPath));
    }else{//不在线、显示灰色图标
        QBitmap bmp;
        bmp.load(iconPath);
        ui->pb_icon->setIcon(bmp);
    }
    //3、立即重绘
    this->repaint();
}

void UserItem::setUserOffline()
{
    //1、修改用户状态
    m_state = STATUS_OFFLINE;
    //2、设置头像为灰色
    QString iconPath = QString(":/icon/%1.png").arg(m_iconId);
    QBitmap bmp;
    bmp.load(iconPath);
    ui->pb_icon->setIcon(bmp);
    //3、立即重绘
    this->repaint();
}

void UserItem::on_pb_icon_clicked()
{
    //给kernel发信号，显示窗口
    Q_EMIT SIG_userClicked(m_id);
}

