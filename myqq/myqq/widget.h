#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QLineEdit>
#include <QColor>

class toolbox1;
class CameraMainWin;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(const QIcon icon, int ID, QString name, toolbox1 *w, QWidget *parent = 0);
    virtual ~Widget();
    void add_msg(QString delivername, QString msg); //设置消息框中的内容,谁发的消息、消息的内容
    QPushButton *pushButton;    //发送

    void startCamera();
private slots:
    void on_pushButton_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_toolButton_1_clicked(bool checked);
    void on_toolButton_2_clicked(bool checked);
    void on_toolButton_3_clicked(bool checked);
    void on_toolButton_4_clicked();
    void on_lineEdit_returnPressed();
    void on_toolButton_clicked();
    void on_toolButton_5_clicked();

private:
    void init_widget();

    QTextBrowser *textBrowser;  //显示消息内容
    QFontComboBox *fontComboBox;    //字体
    QComboBox *comboBox;            //字号
    QToolButton *toolButton_1;  //加粗
    QToolButton *toolButton_2;  //倾斜
    QToolButton *toolButton_3;  //下划线
    QToolButton *toolButton_4;  //颜色
    QToolButton *toolButton;    //显示主窗口
    QToolButton *toolButton_5;  //视频聊天

    QLineEdit *lineEdit;        //发送消息的edit


    int userID;
    QString username;
    QColor color;
    toolbox1 *main_w;

    CameraMainWin* camera;
};
#endif // WIDGET_H
