#include "widget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QDebug>

#define TOFILE
static bool flag = true;

struct WAVHEADER
{
    // RIFF 头
    char RiffName[4];
    unsigned long nRiffLength;
    // 数据类型标识符
    char WavName[4];
    // 格式块中的块头
    char FmtName[4];
    unsigned long nFmtLength;
    // 格式块中的块数据
    unsigned short nAudioFormat;
    unsigned short nChannleNumber;
    unsigned long nSampleRate;
    unsigned long nBytesPerSecond;
    unsigned short nBytesPerSample;
    unsigned short nBitsPerSample;
    // 数据块中的块头
    char    DATANAME[4];
    unsigned long   nDataLength;
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _startBtn = new QPushButton(QStringLiteral("开始录音"),this);
    _endBtn = new QPushButton(QStringLiteral("结束录音"),this);
    QHBoxLayout *_hBoxLayout = new QHBoxLayout(this);
    _hBoxLayout->addWidget(_startBtn);
    _hBoxLayout->addWidget(_endBtn);
    //设置格式
    QAudioFormat audioFormat;
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setChannelCount(1);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setSampleRate(16000);
    audioFormat.setSampleSize(16);
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    //判断设备，查看是否存在
    QAudioDeviceInfo devInfo = QAudioDeviceInfo::defaultInputDevice();
    if(devInfo.isNull()){
        qDebug() << "未找到录音设备";
    }
    //不支持格式，使用最接近格式
    if(!devInfo.isFormatSupported(audioFormat)){ //当前使用设备是否支持
        audioFormat = devInfo.nearestFormat(audioFormat); //转换为最接近格式
    }
    _audioInput = new QAudioInput(devInfo,audioFormat,this);
    //内存的IO对象
    bufDevice.setBuffer(&voiceData);
    connect(_startBtn,&QPushButton::clicked,[&]()->void{
             #ifdef TOFILE //存到文件
                outFile.setFileName(qApp->applicationDirPath() + "/test.wav"); //语音原始文件
                outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
                _audioInput->start(&outFile);
                if(!flag)
                    return;
                flag = true;
            #else //存到内存
                bufDevice.open(QIODevice::WriteOnly | QIODevice::Truncate);
                _audioInput->start(&bufDevice);
            #endif
    });
    connect(_endBtn,&QPushButton::clicked,[&]()->void{
                QIODevice *device{nullptr};
                #ifdef TOFILE //存到文件
                    device = &outFile;
                #else //存到内存
                    device = &bufDevice;
                #endif

        //添加wav文件头
                static WAVHEADER wavHeader;
                qstrcpy(wavHeader.RiffName,"RIFF");
                qstrcpy(wavHeader.WavName,"WAVE");
                qstrcpy(wavHeader.FmtName,"fmt ");
                qstrcpy(wavHeader.DATANAME,"data");
                wavHeader.nFmtLength = 16;
                int nAudioFormat = 1;
                wavHeader.nAudioFormat = nAudioFormat;
                wavHeader.nBitsPerSample = 16;
                wavHeader.nChannleNumber = 1;
                wavHeader.nSampleRate = 16000;
                wavHeader.nBytesPerSample = wavHeader.nChannleNumber * wavHeader.nBitsPerSample / 8;
                wavHeader.nBytesPerSecond = wavHeader.nSampleRate * wavHeader.nChannleNumber *  wavHeader.nBitsPerSample / 8;
                wavHeader.nRiffLength = device->size() - 8 + sizeof(WAVHEADER);
                wavHeader.nDataLength = device->size();
        //写到IO设备头
                device->seek(0);
                device->write(reinterpret_cast<char*>(&wavHeader),sizeof(WAVHEADER));
                flag = false;
                _audioInput->stop();
                outFile.close();
            });
}

Widget::~Widget()
{
#ifdef TOFILE
    outFile.close();
#else
     bufDevice.close();
#endif
}
