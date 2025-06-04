#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include <QString>

namespace Ui {
class chatdialog;
}

class chatdialog : public QWidget
{
    Q_OBJECT

public:
    explicit chatdialog(QWidget *parent = nullptr);
    ~chatdialog();
    // 设置窗口信息
    void setInfo(QString name, int id);
    // 设置聊天内容到窗口上
    void setContent(QString content);
    //设置好友不在线到窗口上
    void setFriendOffline();

signals:
    //发送聊天内容和ip给kernel
    void SIG_sendMsg(QString content, int id);

private slots:
    void on_pb_send_clicked();

private:
    Ui::chatdialog *ui;
    QString m_name;
    int m_id;
};

#endif // CHATDIALOG_H
