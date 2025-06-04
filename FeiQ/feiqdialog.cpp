#include "feiqdialog.h"
#include "ui_feiqdialog.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>

FeiQDialog::FeiQDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FeiQDialog)
{
    ui->setupUi(this);
}

FeiQDialog::~FeiQDialog()
{
    delete ui;
}
//添加一个好友
void FeiQDialog::addFriend(QString ip, QString name)
{
    //1、在原有的基础上添加一行
    ui->tw_friend->setRowCount(ui->tw_friend->rowCount() + 1);
    //2、添加第0列单元格
    QTableWidgetItem* item0 = new QTableWidgetItem(ip);
    ui->tw_friend->setItem(ui->tw_friend->rowCount() - 1, 0, item0);
    //3、添加第一列单元格
    QTableWidgetItem* item1 = new QTableWidgetItem(name);
    ui->tw_friend->setItem(ui->tw_friend->rowCount() - 1, 1, item1);
}
//从列表上删除一个好友
void FeiQDialog::deleteFriend(QString ip)
{
    for(int i = 0; i < ui->tw_friend->rowCount(); ){
        if(ip == ui->tw_friend->item(i, 0)->text()){//地址相等就删除
            ui->tw_friend->removeRow(i);
        }else{//地址不相等，就看下一行
            ++i;
        }
    }
    //重绘
    this->update();
}

void FeiQDialog::closeEvent(QCloseEvent *event)
{
    //1、忽略关闭事件, 不走父类的关闭窗口
    event->ignore();
    //2、弹出一个提示窗口
    if(QMessageBox::Yes == QMessageBox::question(this, "提示", "是否确认关闭")){
        //给kernel发关闭窗口的信号
        Q_EMIT SIG_closeDialog();
    }
}


void FeiQDialog::on_tw_friend_cellDoubleClicked(int row, int column)
{
    //发消息通知kerner，点击了哪个好友
    //取出当前双击的那一行的IP地址
    QString ipStr = ui->tw_friend->item(row, 0)->text();
    Q_EMIT SIG_userClicked(ipStr);
}

