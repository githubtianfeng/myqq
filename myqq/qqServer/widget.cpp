#include "widget.h"
#include <QHostAddress>
#include <QDebug>
#include <QByteArray>
#include <QThread>

#pragma execution_character_set("utf-8")

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(200,150);
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,8099);
    for(int i=0;i<CLIENTCOUNT;++i)
    {
        //指针数组必须初始化 不然会报错
        socket_client[i] = NULL;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(getNewConnection()));

}

Widget::~Widget()
{

}

int Widget::getClientFromList(QObject *p)
{
    for(int i=0;i<clientList.size();i++)
    {
        if(clientList.at(i) == p)
        {
            return i;
        }
    }
    return -1;
}

void Widget::getNewConnection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();

    qDebug() << "get a new client!";

    connect(socket,SIGNAL(readyRead()),this,SLOT(readDataFromClient()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(removeSockets()));
}

void Widget::readDataFromClient()
{
//    int index = getClientFromList(sender());
//    if(index == -1)
//        return;

    QTcpSocket* sockClient = (QTcpSocket*)sender();
    QByteArray msg = sockClient->readAll();
  //  qDebug() << "index:" <<index <<" read:" << msg;
    qDebug() << msg;
    if(msg.startsWith("#Chat:"))//聊天
    {
        sendmsg(msg);
    }
    else if(msg.startsWith("#Login"))//登录
    {
        loginmsg(sockClient,msg);
    }
    else if(msg.startsWith("#Camera:"))
    {
        sendMsgCamera(msg);
    }
    else if(msg.contains("#Cam_confirm:"))//服务器收到视频响应
    {
        sendConfirmToClient(msg);
    }
}


void Widget::removeSockets()
{
//    int index = getClientFromList(sender());
//    if(index == -1)
//        return;

    QTcpSocket* sockClient = (QTcpSocket*)sender();
    //socket_client中去除
    for (int i = 0; i < CLIENTCOUNT; i++) //向socket_client数组中每个client广播用户状态消息
    {
        if (socket_client[i] == sockClient)
        {
            socket_client[i] = NULL;
            broadcast_user_status(i,0); //向socket_client数组中所有socket广播用户状态消息
            break;
        }
    }
    //clientList中去除
//    if(index < clientList.size())
//    {
//        clientList.at(index)->deleteLater();//先释放资源 再removeAt
//        clientList.removeAt(index);
//    }
    for(int i=0;i<clientList.size();i++)
    {
        if(clientList.at(i) == sockClient)
        {
            sockClient->deleteLater();//先释放资源 再removeAt
            clientList.removeAt(i);
        }
    }
}

void Widget::loginmsg(QTcpSocket *sockClient, QByteArray msg)
{
 //   #Login:1,passwd  1号用户登录 密码passwd
    msg = msg.remove(0,7);//删除#Login:
    QList<QByteArray> arrList = msg.split(',');
    int o_userid = arrList.at(0).toInt();
    QByteArray passwd = arrList.at(1);

    if ((o_userid < 0) || (o_userid >= CLIENTCOUNT)) //无效的userid
    {
        qDebug("login failed, %d:invalid userid",o_userid);
        QByteArray err("#Error,1");
        sockClient->write(err);
        return;
    }
    if (!auth_passwd(o_userid, passwd))//验证用户登录UserId和密码
    {
        qDebug("login failed, userid=%d,passwd=%s:invalid password\n",o_userid,passwd);
        QByteArray err("#Error,2");
        sockClient->write(err);
        return;
    }
    qDebug("%d:login success",o_userid);
    broadcast_user_status(sockClient);
    clientList.push_back(sockClient);
    fix_socket_client(o_userid,sockClient);
    broadcast_user_status(o_userid,1); //向socket_client数组中所有socket广播用户状态消息
}

int Widget::auth_passwd(int userid, QByteArray passwd)
{
    if (passwd == "123456")//如果密码等于"123456"验证通过
        return 1;
    else
        return 0;//密码验证失败
}

void Widget::fix_socket_client(int index, QTcpSocket *sockClient)
{
    if(socket_client[index]) //同一个userid没有下线，却又在另一个终端登录，拒绝登录
    {
        qDebug("%d:userid already login\n",index);
        QByteArray err("#Error,3");
        sockClient->write(err);//给client端socket下发系统消息
    }
    else
    {
        //如果socket_client[index] 等于NULL，将client端socket赋予socket_client[index]
        socket_client[index] = sockClient;
    }
}

void Widget::broadcast_user_status(int userId,int status)
{
    //把之前登录的用户的信息（服务器中保留的在线用户信息）发给新登录的客户端
    //之前下线的用户没必要发送 因为已经从服务器中移除了
    for(int i=0;i<clientList.size();i++)
    {
        //把新登录的用户的在线信息通知给所有在线用户
        if(status == 0 && (clientList.at(i) == socket_client[userId]))//如果是1下线 那就不要给1发送消息
            return;
        QByteArray msg("#Broadcast:");
        msg.append(QByteArray::number(userId)).append(",").append(QByteArray::number(status));
        clientList.at(i)->write(msg);
        clientList.at(i)->waitForBytesWritten();
    }  
}

void Widget::broadcast_user_status(QTcpSocket *socketClient)
{
    //把之前登录的用户的信息（服务器中保留的在线用户信息）发给新登录的客户端
    //之前下线的用户没必要发送 因为已经从服务器中移除了
    int count =0 ;
    for(int i=0;i < CLIENTCOUNT;i++)
    {
        if(count >= clientList.size())
            break;
        if(socket_client[i])
        {
            count++;
            QByteArray msg("#Broadcast:");
            msg.append(QByteArray::number(i)).append(",1");
            socketClient->write(msg);

            //等待socket上至少写一个字节才会断开阻塞
            //否则会出现 "#Broadcast:2,1#Broadcast2,0" 这样不同的socket中字节拼接的情况
            if(socketClient->ConnectedState)
            {
                socketClient->waitForBytesWritten();
            }
            QThread::usleep(200000);//延迟200ms发送 usleep单位是千分之1毫秒
        }
    }
}

void Widget::sendmsg(QByteArray msg)
{
    QByteArray str = msg;
    str.remove(0,6);//删除#Chat:
    QList<QByteArray> arrList = str.split(',');
    QTcpSocket* sockClient = socket_client[arrList.at(1).toInt()];
    if (sockClient == NULL) //没有这个目标用户
    {
        qDebug() << arrList.at(1) << ":have not this userid";
    }
    else
    {
        //给client端socket下发send消息
        sockClient->write(msg);

        //服务器如果要输出中文 就要加以下代码  并且
//        QTextCodec* codec = QTextCodec::codecForName("utf-8");
//        QString arr =  codec->toUnicode(msg);
        qDebug() << arrList.at(0).toInt() << "to" << arrList.at(1).toInt() <<":" << arrList.at(2);
    //    qDebug("send message:%s to %s:%s\n",arrList.at(0).data(),arrList.at(1).data(),arrList.at(2).data());
    }
}

void Widget::sendMsgCamera(QByteArray msg)
{
    //#Camera:1,2
    QByteArray str = msg;
    str.remove(0,8);//删除#Camera:
    QList<QByteArray> arrList = str.split(',');
    QTcpSocket* sockClient = socket_client[arrList.at(1).toInt()];
    if (sockClient == NULL) //没有这个目标用户
    {
        qDebug("%d:have not this userid\n",arrList.at(1));
    }
    else
    {
        //给client端socket下发send消息
        sockClient->write(msg);
    }
}

void Widget::sendConfirmToClient(QByteArray msg)
{
    //#Cam_confirm:2,1  2号发给1号 同意
    //所以服务器应该发给1号同意的消息
    QByteArray arr = msg;
    arr.remove(0,13);//删除#Cam_confirm:
    QList<QByteArray> arrList = arr.split(',');
    QTcpSocket* sockClient = socket_client[arrList.at(1).toInt()];
    if(sockClient)
    {
        sockClient->write(msg);
        qDebug() << "sendConfirmToClient" << msg;
    }
}
