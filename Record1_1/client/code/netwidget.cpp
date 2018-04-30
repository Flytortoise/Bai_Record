#include "netwidget.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QGridLayout>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QApplication>


const qint64 LOADBYTES = 1024;  //每次发送的大小
NetWidget::NetWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(tr("网络连接"));
    CreateInit();
    CreateLayout();
    this->setFixedSize(QWidget::sizeHint());
}

void NetWidget::CreateInit()
{
    const char *IPREGEXP = "((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))";
    const char *PORTREGEXP = "0|[1-9]\\d{0,3}|[1-5]\\d{4}|6[0-5][0-5][0-3][0-5]";

    QRegExp IPRegExp(IPREGEXP);
    QRegExp PortRegExp(PORTREGEXP);

    IPLabel = new QLabel(tr("IP:"));
    IPLineEdit = new QLineEdit(tr("120.79.209.96"));
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
    });

    DisConnectBtn = new QPushButton(tr("断开连接"));
    DisConnectBtn->setEnabled(false);
    connect(DisConnectBtn, &QPushButton::clicked,
            [this](){
        ClientSocket->close();
        ConnectBtn->setEnabled(true);
        DisConnectBtn->setEnabled(false);
        SendFileBtn->setEnabled(false);
    });

    FilePathLabel = new QLabel;
    SendProgressBar = new QProgressBar;
    SendProgressBar->setMinimum(0);

    SendFileBtn = new QPushButton(tr("发送文件"));
    SendFileBtn->setEnabled(false);
    connect(SendFileBtn, SIGNAL(clicked(bool)),
            this, SLOT(SendFileName()));

    SelectFileBtn = new QPushButton(tr("选择文件"));
    connect(SelectFileBtn, &QPushButton::clicked,
            [this](){
        FilePathName = QFileDialog::getOpenFileName(this);
        if(!FilePathName.isEmpty())
        {
            FilePathLabel->setText(FilePathName);
            SendProgressBar->setValue(0);
        }
    });

    connect(ClientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(showError(QAbstractSocket::SocketError)));

    connect(ClientSocket, &QTcpSocket::connected,
            [this](){
        ConnectBtn->setEnabled(false);
        DisConnectBtn->setEnabled(true);
        SendFileBtn->setEnabled(true);
    });

    //发送第一个数据包之后，QTcpSocket会发出这个信号
    connect(ClientSocket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(SendFileData(qint64)));

    connect(ClientSocket, SIGNAL(readyRead()),
            this, SLOT(RecvMessage()));

    file = nullptr;
    loadBytes = LOADBYTES;

    resultTextEdit = new QTextEdit;
    QFont font;
    font.setPointSize(12);
    resultTextEdit->setFont(font);
    resultTextEdit->setReadOnly(true);

    _ClearWav = new QPushButton(tr("清除音频缓存"));
    connect(_ClearWav, &QPushButton::clicked,
            [this](){
       QString order = QString("%1/*").arg(qApp->applicationDirPath()+QString("/wav"));
       order = QString("del /q/f ") + order.replace("/","\\");
       system(order.toStdString().c_str());
    });
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

    mainLayout->addWidget(FilePathLabel, 2, 0, 1, 3);
    mainLayout->addWidget(SelectFileBtn, 2, 3);
    mainLayout->addWidget(SendProgressBar, 3, 0, 1, 3);
    mainLayout->addWidget(SendFileBtn, 3, 3);

    mainLayout->addWidget(resultTextEdit, 4, 0, 1, 4);
    mainLayout->addWidget(_ClearWav, 5, 3);
}

void NetWidget::showError(QAbstractSocket::SocketError )
{
    QString strErr = ClientSocket->errorString();
    QMessageBox::critical(this,tr("网络错误"),
                          tr("产生如下错误:%1").arg(strErr));
    ConnectBtn->setEnabled(true);
    DisConnectBtn->setEnabled(false);
    SendFileBtn->setEnabled(false);
    ClientSocket->close();
}

void NetWidget::SendFileName()
{
    SendFileBtn->setEnabled(false);
    if(FilePathName.isEmpty()){
        QMessageBox::critical(this, tr("文件错误"),
                              tr("未指定文件!"), Qt::NoButton, Qt::NoButton);
        return;
    }

    if(file != nullptr)
        delete file;

    file = new QFile(FilePathName);

    if(!file->open(QFile::ReadOnly)){
        QMessageBox::critical(this, tr("文件错误"),
                              tr("文件打开失败！"), Qt::NoButton, Qt::NoButton);
        return;
    }

    sentBytes = 0;
    fileBytes = file->size();
    SendProgressBar->setValue(0);
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);

    //无路径文件名
    QString fileName = FilePathName.right(FilePathName.size() -
                                   FilePathName.lastIndexOf('/') - 1);
    //数据包 = 总大小+文件名长度+文件名，先占位
    out << qint64(0) << qint64(0) << fileName;

    fileBytes += buff.size();
    SendProgressBar->setMaximum(fileBytes);

    out.device()->seek(0);  //重写总大小和文件名长度
    out << fileBytes << (qint64(buff.size()) - 2 * sizeof(qint64)); //减掉两个qint64
    //ClientSocket->write("_work_");
    restBytes = fileBytes - ClientSocket->write(buff);  //发送数据包
/*
    qDebug() << fileBytes;
    qDebug() << (qint64(buff.size()) - 2 * sizeof(qint64));
    qDebug() << fileName;
    qDebug() << buff.toHex();
*/

}

void NetWidget::SendFileData(qint64 sentSize)
{

    sentBytes += sentSize;
    SendProgressBar->setValue(sentBytes);

    if(restBytes > 0)
    {
        QByteArray buff = file->read(qMin(loadBytes, restBytes));
        restBytes -= ClientSocket->write(buff);
    }
    else
    {
        file->close();
        resultTextEdit->clear();
        resultTextEdit->append("服务器解析中....");
    }

}

void NetWidget::RecvMessage()
{
    QByteArray recvdata = ClientSocket->readAll();
    QJsonParseError json_error;
    //QJsonObject tmpObject;
    QJsonValue tmpValue;
    QJsonDocument parse_document = QJsonDocument::fromJson(recvdata, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_document.isObject())
        {
            QJsonObject obj = parse_document.object();
            if(obj.contains("results_recognition"))
            {
                tmpValue = obj.take("results_recognition");
                QJsonArray result = tmpValue.toArray();
                parsed_text = result.at(0).toString();
                raw_text = result.at(1).toString();
            }
        }
    }
    else
    {
        QMessageBox::critical(this, tr("接受错误！"), json_error.errorString());
    }

    //emit RecvReady();
    GetResult();

    SendFileBtn->setEnabled(true);
}

QString NetWidget::GetResult()
{
    QString result;
    if(!raw_text.isEmpty())
        result = QString("解析结果:\n%1 \n或: \n%2").arg(parsed_text,raw_text);
    else
        result = QString("解析结果:\n%1").arg(parsed_text);

    resultTextEdit->append(result);
    return result;
}

void NetWidget::GetRecord(QString fileName)
{
    FilePathLabel->setText(fileName);
    FilePathName.clear();
    FilePathName.append(fileName);
    if(SendFileBtn->isEnabled())
        SendFileName();
}

void NetWidget::CloseConnect()
{
    if(!ClientSocket->isOpen())
        return;

    ClientSocket->close();
}









