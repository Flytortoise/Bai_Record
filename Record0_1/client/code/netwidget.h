#ifndef NETWIDGET_H
#define NETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>

class NetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetWidget(QWidget *parent = 0);

signals:

public slots:
    void showError(QAbstractSocket::SocketError);
    void SendFileName();
    void SendFileData(qint64);

private:
    void CreateInit();
    void CreateLayout();

private:
    QLabel *IPLabel;
    QLabel *PortLabel;

    QLineEdit *IPLineEdit;
    QLineEdit *PortLineEdit;

    QPushButton *ConnectBtn;
    QPushButton *DisConnectBtn;

    QTcpSocket *ClientSocket;

    QLabel *FilePathLabel;
    QProgressBar *SendProgressBar;
    QPushButton *SelectFileBtn;
    QPushButton *SendFileBtn;

    QString FilePathName;
    QFile *file;
    qint64 fileBytes, sentBytes, restBytes, loadBytes;
    //总数据大小，已发送数据大小，剩余数据大小，每次发送数据块的大小
};

#endif // NETWIDGET_H
