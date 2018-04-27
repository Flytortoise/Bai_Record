#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("语音翻译"));
    CreateInit();
    CreateLayout();
}

void MainWindow::CreateInit()
{
    netwidget = new NetWidget;
    recordwidget = new RecordWidget;
    connect(recordwidget, SIGNAL(RecordOk(QString)),
            netwidget, SLOT(GetRecord(QString)));

    recAction = new QAction("录音翻译");
    connect(recAction, &QAction::triggered,
            [this](){
       if(recordwidget->isHidden())
           recordwidget->show();
    });


}

void MainWindow::CreateLayout()
{
    this->setCentralWidget(netwidget);
    this->menuBar()->addAction(recAction);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    netwidget->CloseConnect();
    recordwidget->close();
}

MainWindow::~MainWindow()
{

}
