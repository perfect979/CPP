#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    //重写父类的关闭窗口事件
    void closeEvent(QCloseEvent* event);

private slots:
    void on_pb_clear_clicked();

    void on_pb_commit_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_register_clicked();


signals:
    //把注册信息发给kernel
    void SIG_registerCommit(QString tel, QString name, QString password);
    //把登录信息发给kernel
    void SIG_loginCommit(QString tel, QString password);
    //通知kernel关闭登录和注册窗口
    void SIG_closeLogindig();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
