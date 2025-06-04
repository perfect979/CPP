#include "mychatdialog.h"
#include "ui_mychatdialog.h"
#include <QDebug>
#include <QMessageBox>

MyChatDialog::MyChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MyChatDialog)
{
    ui->setupUi(this);
    setWindowTitle("聊天软件 V1.0");
    m_layout = new QVBoxLayout;
    //设置距离外控件上下左右的间距
    m_layout->setContentsMargins(0,0,0,0);
    //设置里面每个空间彼此间的距离
    m_layout->setSpacing(3);
    //把层设置到外部的空间上
    ui->tw_list->setLayout(m_layout);

    //添加菜单
    m_menu = new QMenu(this);
    //往菜单中添加菜单项
    m_menu->addAction("系统设置");
    m_menu->addAction("添加好友");
    //绑定点击菜单项的信号和槽函数
    connect(m_menu, SIGNAL(triggered(QAction*)),
            this, SLOT(SLOT_dealMenu(QAction*))
            );
}

MyChatDialog::~MyChatDialog()
{
    delete ui;
}

void MyChatDialog::closeEvent(QCloseEvent *event)
{
    //忽略关闭事件，不走父类的关闭窗口
    event->ignore();
    if(QMessageBox::Yes == QMessageBox::question(this, "提示", "是否确认退出？")){
        //1、回收类里的资源（当前类没有其他资源）
        if(m_layout){
            delete m_layout;
            m_layout = nullptr;
        }
        if(m_menu){
            delete m_menu;
            m_menu = nullptr;
        }
        //2、通知kernel关闭应用程序
        Q_EMIT SIG_closeMyChatdlg();
    }
}

void MyChatDialog::addFriend(UserItem *item)
{
    //往层上添加控件
    m_layout->addWidget(item);
}

void MyChatDialog::deleteFriend(UserItem *item)
{

}

void MyChatDialog::setUserInfo(QString name, QString feeling, int iconId)
{
    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/icon/%1.png").arg(iconId)));
}


void MyChatDialog::on_pb_menu_clicked()
{
    //获取当前鼠标点击位置
    QPoint p = QCursor::pos();
    //计算菜单项的高度
    QSize size = m_menu->sizeHint();
    //在点击的位置向上显示一个菜单列表
    m_menu->exec(QPoint(p.x(), p.y() - size.height()));
}

void MyChatDialog::SLOT_dealMenu(QAction *action)
{
    if("添加好友" == action->text()){
        //给kernel发消息，通知kernel要添加好友
        Q_EMIT SIG_addFriend();
    }else if("系统设置" == action->text()){
        qDebug() << "系统设置";
    }
}



