#include <QApplication>
#include "toolbox1.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    toolbox1 w;
    w.resize(300,600);
    w.show();
    return a.exec();
}
