#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("IM V1.0");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    //1、回收类里的资源

    //2、通知Kernel关闭应用程序
    Q_EMIT SIG_closeLogindig();
}

void LoginDialog::on_pb_clear_clicked()
{
    ui->le_tel->setText("");
    ui->le_password->setText("");
}


void LoginDialog::on_pb_commit_clicked()
{
    //1、从控件上获取用户输入的数据
    QString tel = ui->le_tel->text();
    QString password = ui->le_password->text();
    //2、校验用户输入的数据
    //校验规则：
    //2.1、不能为空，不能之包含空格
    //2.2、校验长度，手机号必须是11位，昵称不能超过10，密码不能超过15
    //2.3、校验内容，手机号必须满足一定规则的数字，--使用正则表达式（目前美学，暂时不写）
    QString telTemp = tel;
    QString passwordTemp = password;
    if (tel.isEmpty() || password.isEmpty() ||
            telTemp.remove(" ").isEmpty() || passwordTemp.remove(" ").isEmpty()){
        QMessageBox::about(this, "提示", "输入为空或空字符串");
        return;
    }
    if(tel.length() != 11 || password.length() > 15){
        QMessageBox::about(this, "提示", "手机号必须是11位，密码不能超过15");
    }

    //3、通过信号把数据发给kernel
    Q_EMIT SIG_loginCommit(tel, password);
}


void LoginDialog::on_pb_commit_register_clicked()
{
    //1、从控件上获取用户输入的数据
    QString tel = ui->le_tel_register->text();
    QString name = ui->le_name_register->text();
    QString password = ui->le_password_register->text();
    qDebug() <<"tel "<<tel<<"\n name "<<name<<"\n password "<<password;
    //2、校验用户输入的数据
    //校验规则：
    //2.1、不能为空，不能之包含空格
    //2.2、校验长度，手机号必须是11位，昵称不能超过10，密码不能超过15
    //2.3、校验内容，手机号必须满足一定规则的数字，--使用正则表达式（目前美学，暂时不写）
    QString telTemp = tel;
    QString nameTemp = name;
    QString passwordTemp = password;
    if (tel.isEmpty() || name.isEmpty() || password.isEmpty() ||
            telTemp.remove(" ").isEmpty() || nameTemp.remove(" ").isEmpty() || passwordTemp.remove(" ").isEmpty()){
        QMessageBox::about(this, "提示", "输入为空或空字符串");
        return;
    }
    if(tel.length() != 11 ||name.length() > 10 || password.length() > 15){
        QMessageBox::about(this, "提示", "手机号必须是11位，昵称不能超过10，密码不能超过15");
    }

    //3、通过信号把数据发给kernel
    Q_EMIT SIG_registerCommit(tel, name, password);
}


void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_tel_register->setText("");
    ui->le_password_register->setText("");
    ui->le_name_register->setText("");
}

