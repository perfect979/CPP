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
    void setInfo(QString ip);
    // 设置聊天内容到窗口上
    void setContent(QString content);

signals:
    //发送聊天内容和ip给kernel
    void SIG_sendMsg(QString content, QString ip);

private slots:
    void on_pb_send_clicked();

private:
    Ui::chatdialog *ui;
    QString m_ip;
};

#endif // CHATDIALOG_H
