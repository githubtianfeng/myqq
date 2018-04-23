#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QTcpSocket>
#include <QCloseEvent>
#include "Music.h"

#define CLINETCOUNT 256  //最大客户端数量
/**
 * @brief The Widget class
 * #Chat:1,2,msg  1发送给2msg
 * #Login:1,passwd  1号用户登录 密码passwd
 * #Error:1  无效登录id
 * #Error:2  无效密码
 * #Error:3  userid already login
 * #Broadcast:2,1  用户2  1在线  0下线
 * #Camera:1,2  1向2请求视频聊天
 * #Cam_confirm:2,1  客户端2发给服务器视频确认同意1号用户的请求
 */


class Widget;

class toolbox1 : public QToolBox
{
    Q_OBJECT
public:
    explicit toolbox1(QWidget *parent = 0);
    virtual ~toolbox1();
    void send_Msg(int d_userid, QByteArray arr);
    void send_Request_Camera(int d_userid);
signals:

public slots:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QString username[CLINETCOUNT];  //用户名数组
    QToolButton *toolBtn[CLINETCOUNT];  //主窗口控件
    Widget *child[CLINETCOUNT]; //子控件
    void init_toolBtn();
    void init_username();
    QTcpSocket *sockClient;
    void login_Msg();
    void recv_send_Msg(QByteArray msg);
    void userStatus_Msg(QByteArray msg);
    void system_Msg(QByteArray msg);
    void recv_send_Msg_Camera(QByteArray msg);
    void recv_send_ConfirmToClient(QByteArray msg);
    int userid;
    QString passwd;
    QString hostip;
    int hostport;
    void closeEvent(QCloseEvent *event);
    Music* m_music;
private slots:
    void sock_Error(QAbstractSocket::SocketError sockErr);
    void read_Msg();
    void socket_connected();
    void on_music_clicked();
};

#endif // MAINWINDOW_H
