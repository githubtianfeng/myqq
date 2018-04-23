#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#define CLIENTCOUNT 256

/**
 * @brief The Widget class
 * #Chat:1,2,msg  1发送给2msg
 * #Login:1,passwd  1号用户登录 密码passwd
 * #Error:1  无效登录id
 * #Error:2  无效密码
 * #Error:3  userid already login
 * #Broadcast:2,1  用户2  1在线  0下线
 * #Camera:1,2  1向2请求视频聊天
 * #Cam_confirm:1  客户端发给服务器视频响应1号用户的请求
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    //获取客户端
    int getClientFromList(QObject* p);
    //发送聊天信息给所有客户端
    void sendChatMsgToAllClient(QByteArray msg);
public slots:
    void getNewConnection();
    void readDataFromClient();//接受消息
    void removeSockets();//清理socket

    void loginmsg(QTcpSocket* sockClient, QByteArray msg);
    int auth_passwd(int userid, QByteArray passwd);//验证用户密码
    void fix_socket_client(int index, QTcpSocket* sockClient); //将accept的客户端连接安装到socket_client[10]的数组中
    void broadcast_user_status(int userId,int status); //向socket_client[]数组中所有client广播用户状态消息
    void broadcast_user_status(QTcpSocket* socketClient);
    void sendmsg(QByteArray msg); //处理send消息
    void sendMsgCamera(QByteArray msg);//处理视频聊天请求
    void sendConfirmToClient(QByteArray msg);//服务器收到视频响应

private:
    QTcpServer* tcpServer;
    QList<QTcpSocket*> clientList;
    QTcpSocket* socket_client[CLIENTCOUNT]; //申明socket_client数组，管理client的socket连接

};

#endif // WIDGET_H
