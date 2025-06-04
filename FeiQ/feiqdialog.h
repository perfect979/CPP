#ifndef FEIQDIALOG_H
#define FEIQDIALOG_H

#include <QDialog>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class FeiQDialog; }
QT_END_NAMESPACE

class FeiQDialog : public QDialog
{
    Q_OBJECT

public:
    FeiQDialog(QWidget *parent = nullptr);
    ~FeiQDialog();
    //添加一个好友
    void addFriend(QString ip, QString name);
    //删除一个好友
    void deleteFriend(QString ip);
    //重写关闭事件
    void closeEvent(QCloseEvent* event);
signals:
    //发送双击那一行的好友的ip地址给kernel
    void SIG_userClicked(QString ip);
    //关闭窗口的信号
    void SIG_closeDialog();

private slots:
    void on_tw_friend_cellDoubleClicked(int row, int column);

private:
    Ui::FeiQDialog *ui;
};
#endif // FEIQDIALOG_H
