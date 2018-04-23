#include <QMessageBox>
#include <QHostAddress>
#include "toolbox1.h"
#include "logindlg.h"
#include "widget.h"
#include <QPushButton>
#include <QDebug>
#include <QList>
#include <QTextCodec>
#include <QMessageBox>

//主窗口的信号过滤函数，watched代表是哪个控件触发了信号，event代表触发了具体什么信号
bool toolbox1::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)//如果是鼠标按键信号
    {
        int i = watched->objectName().toInt();
        child[i]->hide();
        child[i]->showNormal();//将toolbtn下对应的child显示到屏幕
    }
    return QToolBox::eventFilter(watched, event);//其他信号交给父类进行默认处理
}

void toolbox1::init_username()
{
    for(int i = 0;i<CLINETCOUNT;i++ )//将username[]数组中所有的成员初始化
    {
        username[i] = tr("用户") + QString::number(i);
    }
}

void toolbox1::init_toolBtn()//初始化所有的toolBtn
{
    QString imagename;
    for(int i = 0;i<CLINETCOUNT;i++ )
    {
        toolBtn[i] = new QToolButton();
        toolBtn[i]->setObjectName(QString::number(i));//为每个toolBtn设置ObjectName属性
        toolBtn[i]->installEventFilter(this);//toolBtn的点击事件由toolbox1来处理
        toolBtn[i]->setText(username[i]);//设置toolBtn标题
        imagename.sprintf(":/images/%d.png", i);
        toolBtn[i]->setIcon(QPixmap(imagename));//设置toolBtn图像
        toolBtn[i]->setIconSize(QPixmap(imagename).size());//设置toolBtn大小和图像一致
        toolBtn[i]->setAutoRaise(true);//设置toolBtn自动浮起界面风格
        toolBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置toolBtn文字在图像旁边
        toolBtn[i]->setToolTip(tr("未知"));//设置toolBtn 鼠标提示为“未知”
        child[i] = new Widget(toolBtn[i]->icon(), i, toolBtn[i]->text(), this);
    }

    QGroupBox *groupBox[8];
    QVBoxLayout *layout[8];
    static int index = 0;
    QGroupBox* groupBoxTemp;
    QVBoxLayout* layoutTemp;
    QToolButton* button1;
    QToolButton* button2;
    for(int i=0;i<8;i++)
    {
        groupBox[i] = new QGroupBox;
        layout[i] = new QVBoxLayout(groupBox[i]);
        layout[i]->setMargin(10);//设置布局中各窗体的显示间距
        layout[i]->setAlignment(Qt::AlignHCenter);//设置布局中各窗体的位置
        for(int j=0;j<32;j++)
        {
            layout[i]->addWidget(toolBtn[index++]);
        }
        layout[i]->addStretch();//插入一个占位符
    }

    groupBoxTemp = new QGroupBox;
    layoutTemp = new QVBoxLayout(groupBoxTemp);
    layoutTemp->setAlignment(Qt::AlignHCenter);
    button1 = new QToolButton;
    button2 = new QToolButton;
    button1->setText("听音乐");
    layoutTemp->addWidget(button1);
    connect(button1, SIGNAL(clicked()), this, SLOT(on_music_clicked()));
    button2->setText("玩游戏");
    layoutTemp->addWidget(button2);

    //添加item
    addItem((QWidget *)groupBox[0], tr("好友"));
    addItem((QWidget *)groupBox[1], tr("陌生人"));
    addItem((QWidget *)groupBox[2], tr("家人"));
    addItem((QWidget *)groupBox[3], tr("同学"));
    addItem((QWidget *)groupBox[4], tr("同事"));
    addItem((QWidget *)groupBox[5], tr("网友"));
    addItem((QWidget *)groupBox[6], tr("驴友"));
    addItem((QWidget *)groupBox[7], tr("社区"));
    addItem((QWidget *)groupBoxTemp,tr("娱乐"));
}


toolbox1::toolbox1(QWidget *parent) :
    QToolBox(parent)
{
    setWindowIcon(QPixmap(":/images/3.png"));   //设置窗口图标为qq图标
    init_username();//--------------设置用户名------------------
    init_toolBtn();//--------------初始化主窗口控件------------------

    //-------------弹出登录对话框------------------------------
    loginDlg login;
    login.exec();

    if(login.islogin)//登录对话框中用户点击登录按钮
    {
        if((login.userid < 0) ||(login.userid >= CLINETCOUNT))//如果用户输入的userid小于0，大于255，提示错误，程序退出
        {
            QMessageBox::information(this, tr("错误"), tr("无效用户ID号"));
            exit(0);
        }
        setWindowTitle(username[login.userid]);//根据登录对话框中用户输入的ID号设置主窗口标题.

        userid = login.userid;
        passwd = login.passwd;
        hostip = login.hostip;
        hostport = login.hostport;
    //    qDebug() << passwd;

        //根据登录对话框中用户输入的服务器IP和端口号连接到服务器
        sockClient = new QTcpSocket(this);
        QHostAddress hostAddr(hostip);
        sockClient->connectToHost(hostAddr, hostport);

        connect(sockClient, SIGNAL(error(QAbstractSocket::SocketError)), this,
                SLOT(sock_Error(QAbstractSocket::SocketError)));//设置socket出错时的槽函数
        connect(sockClient, SIGNAL(connected()), this, SLOT(socket_connected()));//设置socket完成连接消息的槽函数
        connect(sockClient, SIGNAL(readyRead()), this, SLOT(read_Msg()));//设置socket读消息的槽函数
    }
    else
    {
        exit(0);
    }
}

toolbox1::~toolbox1()
{

}

void toolbox1::sock_Error(QAbstractSocket::SocketError sockErr)//socket出错时调用的槽函数
{
    switch (sockErr)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        QMessageBox::information(this, tr("错误"), sockClient->errorString());
    }
}

void toolbox1::read_Msg()//socket接收到来自server端的消息时调用的槽函数
{
    while(sockClient->bytesAvailable() > 0)
    {
        QByteArray msg = sockClient->readAll();
        qDebug() << msg;

        if(msg.startsWith("#Chat:"))//聊天
        {
            recv_send_Msg(msg);
        }
        else if(msg.startsWith("#Broadcast:"))//来自server的用户状态消息
        {
            userStatus_Msg(msg);
        }
        else if(msg.startsWith("Error:"))//来自server的系统消息
        {
            system_Msg(msg);
        }
        else if(msg.startsWith("#Camera:"))
        {
            recv_send_Msg_Camera(msg);
        }
        else if(msg.startsWith("#Cam_confirm:"))//客户端已收到对方视频同意的信息
        {
            recv_send_ConfirmToClient(msg);
        }
    }

}

void toolbox1::login_Msg()//发送登录消息
{
    QByteArray msg("#Login:");
    msg.append(QByteArray::number(userid)).append(",").append(passwd);

    if(sockClient->isOpen())
    {
        if (sockClient->state() == QAbstractSocket::ConnectedState)
        {
            sockClient->write(msg);
            qDebug() << "send login success" << msg;
        }
    }
}

void toolbox1::recv_send_Msg(QByteArray msg)//来自server的send消息
{
    QTextCodec* codec = QTextCodec::codecForName("utf-8");
    QString arr =  codec->toUnicode(msg);

    arr.remove(0,6);
    qDebug() << arr;
   // msg = msg.remove(0,6);//删除#Chat:
    QList<QString> arrList = arr.split(',');
    int o_userid = arrList.at(0).toInt();

    //o_userid  是谁发送消息给我  就把谁的窗口打开
    child[o_userid]->add_msg(username[o_userid], arrList.at(2));
    child[o_userid]->show();
}

void toolbox1::userStatus_Msg(QByteArray msg)//来自server的deliver消息
{
    //#Broadcast:2,1  用户2  1在线  0下线
    msg = msg.remove(0,11);//删除#Broadcast:
    QList<QByteArray> arrList = msg.split(',');
    qDebug() << msg;
    int i = arrList.at(0).toInt();
    if(arrList.at(1) == "1")
    {
        toolBtn[i]->setToolTip(tr("在线"));   //设置toolBtn 鼠标提示为“在线”
        child[i]->pushButton->setEnabled(true);//设置child窗口中pushButton控件状态为可用
    }
    else
    {
        toolBtn[i]->setToolTip(tr("离线"));//设置toolBtn 鼠标提示为“离线”
        child[i]->pushButton->setEnabled(false);//设置child窗口中pushButton控件状态为不可用
    }
}

void toolbox1::send_Msg(int d_userid, QByteArray arr)//发往server的send消息
{
    //#Chat:1,2,msg  1发送给2msg
    if(sockClient->isOpen())
    {
        if (sockClient->state() == QAbstractSocket::ConnectedState)
        {
            QByteArray msg("#Chat:");
            msg.append(QByteArray::number(userid)).append(",").append(QByteArray::number(d_userid))
                    .append(",").append(arr);
            sockClient->write(msg);
        }
    }
}

void toolbox1::send_Request_Camera(int d_userid)
{
    //#Camera:1,2  1发送给2
    if(sockClient->isOpen())
    {
        if (sockClient->state() == QAbstractSocket::ConnectedState)
        {
            QByteArray msg("#Camera:");
            msg.append(QByteArray::number(userid)).append(",").append(QByteArray::number(d_userid));
            sockClient->write(msg);
        }
    }
}


void toolbox1::closeEvent(QCloseEvent *event)//窗口在close之前会自动调用closeEvent函数
{
    QMessageBox::StandardButton button = QMessageBox::question(this, tr("退出程序"),
                                                               QString(tr("是否退出?")),
                                                               QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        event->accept();  //接受退出信号，程序退出
        if (sockClient->isOpen())
            sockClient->close();

        for(int i=0;i<CLINETCOUNT;i++)
        {
            delete child[i];
        }
    }
    else
    {
        event->ignore();  //忽略退出信号，程序继续运行
    }
}

void toolbox1::system_Msg(QByteArray msg)//来自server的系统消息
{
//    #Error:1  无效登录id
//    #Error:2  无效密码
//    #Error:3  userid already login
    msg = msg.remove(0,7);//删除#Error:
    switch (msg.toInt()) {
    case 0:
        QMessageBox::information(this, tr("来自server的消息"), tr("无法识别的消息"));
        break;
    case 1:
        QMessageBox::information(this, tr("来自server的消息"), tr("无效userid"));
        break;
    case 2:
        QMessageBox::information(this, tr("来自server的消息"), tr("无效密码"));
        break;
    case 3:
        QMessageBox::information(this, tr("来自server的消息"), tr("userid已经登录"));
        break;
    case 4:
        QMessageBox::information(this, tr("来自server的消息"), tr("其他"));
        break;
    default:
        QMessageBox::information(this, tr("来自server的消息"), tr("未知消息"));
    }
}

void toolbox1::recv_send_Msg_Camera(QByteArray msg)
{
    //之前1号请求与2号视频
    //2收到1的视频请求信息 是否接受聊天
    //如果不接受 返回 否则 发送给1同意视频 打开1号的聊天窗口
    if(QMessageBox::information(this,tr("视频"),tr("是否视频聊天"),QMessageBox::Yes,QMessageBox::No)
            == QMessageBox::No)
        return;

    msg.remove(0,8);//删除#Camera:
    QList<QByteArray> arrList = msg.split(',');
    QByteArray arr("#Cam_confirm:");
    //2号用户把自己的id 和 确认消息头拼在一起 发送给1 确实视频
    //#Cam_confirm:2,1  2号发给1号 同意
    arr.append(arrList.at(1)).append(",").append(arrList.at(0));//我要发给服务器 视频响应哪个人的

    sockClient->write(arr);
  //  qDebug() << "send confirm" << arr;
    int d_userid = arrList.at(0).toInt();//1号用户id
    child[d_userid]->startCamera();//打开2号用户里1号用户视频窗口
}

void toolbox1::recv_send_ConfirmToClient(QByteArray msg)
{
    //#Cam_confirm:2,1  2发给1的同意消息
    //此时应该在1中打开2号用户的视频聊天窗口
    msg.remove(0,13);//删除#Cam_confirm:
    QList<QByteArray> arrList = msg.split(',');
    int o_userid = arrList.at(0).toInt();//2号用户id
    child[o_userid]->startCamera();

    qDebug() << "recv confirm" << o_userid;
}

void toolbox1::socket_connected()//socket连接到server端以后调用的槽函数
{
    login_Msg();//向server端发送登录消息
}


void toolbox1::on_music_clicked()
{
    m_music = new Music;
    m_music->show();
}
