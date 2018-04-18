#include "netwidget.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QGridLayout>
#include <QByteArray>
#include <QMessageBox>

NetWidget::NetWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(tr("网络连接"));
    CreateInit();
    CreateLayout();
}

void NetWidget::CreateInit()
{
    const char *IPREGEXP = "((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))";
    const char *PORTREGEXP = "0|[1-9]\\d{0,3}|[1-5]\\d{4}|6[0-5][0-5][0-3][0-5]";

    QRegExp IPRegExp(IPREGEXP);
    QRegExp PortRegExp(PORTREGEXP);

    IPLabel = new QLabel(tr("IP:"));
    IPLineEdit = new QLineEdit(tr("192.168.10.134"));
    IPLineEdit->setValidator(new QRegExpValidator(IPRegExp, this));
    IPLineEdit->setAlignment(Qt::AlignHCenter);

    PortLabel = new QLabel(tr("端口号:"));
    PortLineEdit = new QLineEdit(tr("8000"));
    PortLineEdit->setValidator(new QRegExpValidator(PortRegExp, this));
    PortLineEdit->setAlignment(Qt::AlignRight);

    ClientSocket = new QTcpSocket(this);

    ConnectBtn = new QPushButton(tr("连接服务器"));
    connect(ConnectBtn, &QPushButton::clicked,
            [this](){
        ClientSocket->connectToHost(IPLineEdit->text(),
                                    PortLineEdit->text().toInt());
        ConnectBtn->setEnabled(false);
        DisConnectBtn->setEnabled(true);
        SendBtn->setEnabled(true);
    });

    DisConnectBtn = new QPushButton(tr("断开连接"));
    DisConnectBtn->setEnabled(false);
    connect(DisConnectBtn, &QPushButton::clicked,
            [this](){
        ClientSocket->close();
        ConnectBtn->setEnabled(true);
        DisConnectBtn->setEnabled(false);
        SendBtn->setEnabled(false);
    });

    SendLineEdit = new QLineEdit;
    RecvLineEdit = new QLineEdit;
    SendBtn = new QPushButton(tr("发送信息"));
    SendBtn->setEnabled(false);
    connect(SendBtn, &QPushButton::clicked,
            [this](){
        ClientSocket->write(SendLineEdit->text().toStdString().data());
    });

    connect(ClientSocket, &QTcpSocket::readyRead,
            [this](){
        QByteArray data = ClientSocket->readAll();
        RecvLineEdit->setText(data.toStdString().data());
    });

    connect(ClientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(showError(QAbstractSocket::SocketError)));
}

void NetWidget::CreateLayout()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setHorizontalSpacing(10);

    mainLayout->addWidget(IPLabel, 0, 0);
    mainLayout->addWidget(IPLineEdit, 0, 1);
    mainLayout->addWidget(PortLabel, 0, 2);
    mainLayout->addWidget(PortLineEdit, 0, 3);

    mainLayout->addWidget(ConnectBtn, 1, 0, 1, 2);
    mainLayout->addWidget(DisConnectBtn, 1, 2, 1, 2);

    mainLayout->addWidget(SendLineEdit, 2, 0, 1, 3);
    mainLayout->addWidget(SendBtn, 2, 3);
    mainLayout->addWidget(RecvLineEdit, 3, 0, 1, 3);
}

void NetWidget::showError(QAbstractSocket::SocketError error)
{
    QString strErr = ClientSocket->errorString();
    QMessageBox::critical(this,tr("网络错误"),
                          tr("产生如下错误:%1").arg(strErr));
    ConnectBtn->setEnabled(true);
    DisConnectBtn->setEnabled(false);
    SendBtn->setEnabled(false);
}


















