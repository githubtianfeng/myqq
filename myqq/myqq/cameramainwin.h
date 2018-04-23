#ifndef CAMERAMAINWIN_H
#define CAMERAMAINWIN_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QTimer>
#include <QBuffer>
#include <QImage>
#include <QLabel>

/**
 * @brief The CameraMainWin class
 * Qt5.0版本的QMultimedia包含了许多有媒体相关的类，
 * QCamera类用于打开系统的摄像头设备，
 * QCameraViewfinder用于显示捕获的视频，
 * QCameraImageCapture用于截图
 */
class CameraMainWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit CameraMainWin(QWidget *parent = 0);
    ~CameraMainWin();
    void startCamera();
signals:

public slots:
    void readPendingDatagrams();//udp
    void writeDatagramsToUdpSocket();
    void takeImage(int,QImage image);
    void timerSlot();
private:
    QUdpSocket* udpSocket;

    QCamera* camera;
    QCameraViewfinder* viewfinder;
    QCameraImageCapture* imageCapture;
    QImage image;
    QMediaRecorder* recorder;
    QTimer* timer;

    QLabel* label;
};

#endif // CAMERAMAINWIN_H
