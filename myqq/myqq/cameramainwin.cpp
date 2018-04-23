#include "cameramainwin.h"
#include <QList>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHostAddress>
#include <QImageReader>

/**
 * @brief CameraMainWin::CameraMainWin
 * @param parent
 * UDP通信 本例使用的都是localhost 及本机的不同端口进行通信
 * 真实情况应该是服务器做一个打洞服务，告知两个连接的客户端彼此的id port
 * 两个客户端bind的时候 绑定自己的端口，接收对方的ip地址
 * 发送的时候发送  填写对方的ip地址 端口号
 * 这样一来 两个客户端就可以直接通过udpSocket进行通信
 */
CameraMainWin::CameraMainWin(QWidget *parent) : QMainWindow(parent)
{
    this->resize(600,500);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 8101);

    //摄像头信息获取
//    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
//    foreach (const QCameraInfo &cameraInfo, cameras) {
//        //if (cameraInfo.deviceName() == "/dev/video0") //Linux下选择方式
//          camera = new QCamera(cameraInfo);
//      //    qDebug()<<cameraInfo.deviceName();
//    }
    camera = new QCamera(this);
    //设置摄像头捕获模式
    camera->setCaptureMode(QCamera::CaptureStillImage);
    //图像回显
    viewfinder = new QCameraViewfinder(this);
  //  viewfinder->resize(300,300);
    camera->setViewfinder(viewfinder);

    label = new QLabel("label",this);
    label->setScaledContents(true);
    label->resize(240,180);

    QHBoxLayout* layOut = new QHBoxLayout;
    layOut->addWidget(label);
    layOut->addWidget(viewfinder);
    this->setLayout(layOut);
    this->setCentralWidget(viewfinder);

    //QCameraImageCapture 是获取摄像头捕捉的图片 相关类
    imageCapture = new QCameraImageCapture(camera);
    //绑定 捕获图片 信号 和 处理图片槽函数
    connect(imageCapture,SIGNAL(imageCaptured(int,QImage)),
            this,SLOT(takeImage(int,QImage)));

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

CameraMainWin::~CameraMainWin()
{
    delete camera; delete viewfinder; delete imageCapture;
    if(timer!=NULL)
        delete timer;
}

void CameraMainWin::startCamera()
{
    //启动摄像头
    this->camera->start();

    //启动定时器，用来触发QCameraImageCapture 捕获图片
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    timer->start(500);
    this->show();
}

void CameraMainWin::readPendingDatagrams()
{
    //查看 管道里面是否存在 数据
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray msg;

        //设置数据报 长度
        msg.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(msg.data(),msg.size());
        QBuffer buffer(&msg);
        buffer.open(QIODevice::ReadOnly);
        QImageReader reader(&buffer,"JPEG");
        QImage img = reader.read();
        if(!img.isNull())
        {
            QPixmap pix = QPixmap::fromImage(img);
            pix.scaled(240,180);
            label->setPixmap(pix.scaled(label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }
    }
}

void CameraMainWin::writeDatagramsToUdpSocket()
{

}

void CameraMainWin::takeImage(int, QImage image)
{
    image = image.scaled(240,180);
    //准备 帧头
    QString str =  "\r\n--Guoqi\r\n"
          "Content-Type:image/jpeg\n"
          "Content-Length:%1\n\n";
    str =  str.arg(image.byteCount());
    QByteArray buf;
    buf.clear();
    buf.append(str);

    //将图片存入 缓存中
    QPixmap pic = QPixmap::fromImage(image);
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    pic.save(&buffer,"JPEG");

//    //将缓存 读入 网络字节流后 发出
//    QByteArray dataStr;
//    dataStr.append(buffer.data());
//    qDebug()<<"dataStr(size):"<<dataStr.size();
    buf.append(buffer.data());
    udpSocket->writeDatagram(buf,QHostAddress::LocalHost,8100);
//   // label->setPixmap(pic);
}

void CameraMainWin::timerSlot()
{
    //上锁
    camera->searchAndLock();
    //捕获图片
    imageCapture->capture();
    //解锁
    camera->unlock();
}
