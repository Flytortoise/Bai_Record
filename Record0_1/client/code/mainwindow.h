#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void CreateInit();
    void CreateLayout();

private:
   NetWidget *netwidget;

};

#endif // MAINWINDOW_H
