#ifndef NETWIDGET_H
#define NETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>
#include <QTextEdit>
#include <QCloseEvent>

class NetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetWidget(QWidget *parent = 0);
    QString GetResult();
    void CloseConnect();

signals:
    void RecvReady();

public slots:
    void showError(QAbstractSocket::SocketError);
    void SendFileName();
    void SendFileData(qint64);
    void RecvMessage();
    void GetRecord(QString fileName);

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

    QString parsed_text;
    QString raw_text;

    QTextEdit *resultTextEdit;
};

#endif // NETWIDGET_H
