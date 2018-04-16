#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QAudioInput>
#include <QBuffer>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QPushButton *_startBtn,*_endBtn;//开始和结束按钮
    QAudioInput *_audioInput; //录音对象
    QFile outFile;
    QByteArray voiceData;
    QBuffer bufDevice;
};

#endif // WIDGET_H
