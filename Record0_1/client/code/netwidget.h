#ifndef NETWIDGET_H
#define NETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

class NetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetWidget(QWidget *parent = 0);

signals:

public slots:
    void showError(QAbstractSocket::SocketError);

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

    QLineEdit *SendLineEdit;
    QLineEdit *RecvLineEdit;
    QPushButton *SendBtn;

};

#endif // NETWIDGET_H
