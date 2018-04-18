#include "mainwindow.h"
#include <QVBoxLayout>

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
}

void MainWindow::CreateLayout()
{
    this->setCentralWidget(netwidget);
}

MainWindow::~MainWindow()
{

}
