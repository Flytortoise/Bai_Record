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
    QString GetResult();    //展示服务器发来的结果
    void CloseConnect();    //界面关闭时，关闭连接

signals:
    //void RecvReady();

public slots:
    void showError(QAbstractSocket::SocketError);   //展示错误
    void SendFileName();            //发送文件名
    void SendFileData(qint64);      //发送文件
    void RecvMessage();             //接受服务器发来的结果
    void GetRecord(QString fileName);   //录音结束，自动发送该音频文件

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

    QTcpSocket *ClientSocket;       //socket

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
    QPushButton *_ClearWav;     //清除wav文件夹中的录音文件
};

#endif // NETWIDGET_H
