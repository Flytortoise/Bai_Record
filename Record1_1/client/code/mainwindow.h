#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netwidget.h"
#include <QAction>
#include "recordwidget.h"
#include <QCloseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *);

private:
    void CreateInit();
    void CreateLayout();

private:
   NetWidget *netwidget;
   RecordWidget *recordwidget;
   QAction *recAction;

};

#endif // MAINWINDOW_H
