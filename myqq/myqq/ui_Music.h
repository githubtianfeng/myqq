/********************************************************************************
** Form generated from reading UI file 'Music.ui'
**
** Created: Thu Dec 29 14:17:29 2016
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUSIC_H
#define UI_MUSIC_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QSlider>
#include <QStatusBar>
#include <QToolButton>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_Music
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QSlider *processSlider;
    QPushButton *pushButton;
    QPushButton *clearButton;
    QPushButton *deleteButton;
    QToolButton *previousButton;
    QToolButton *playPauseButton;
    QToolButton *nextButton;
    QToolButton *stoptButton;
    QToolButton *volumeButton;
    QSlider *volumeSlider;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_8;
    QListWidget *songList;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Music)
    {
        if (Music->objectName().isEmpty())
            Music->setObjectName(QString::fromUtf8("Music"));
        Music->resize(885, 619);
        Music->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/bk1.png);"));
        centralwidget = new QWidget(Music);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 10, 211, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Vera Sans"));
        font.setPointSize(18);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("border-image: \\*url();\n"
"color: rgb(255, 255, 255);"));
        processSlider = new QSlider(centralwidget);
        processSlider->setObjectName(QString::fromUtf8("processSlider"));
        processSlider->setGeometry(QRect(0, 490, 891, 21));
        processSlider->setStyleSheet(QString::fromUtf8("border-image: \\*url();"));
        processSlider->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 520, 61, 27));
        pushButton->setStyleSheet(QString::fromUtf8("border-image: \\*url();\n"
"color: rgb(0, 255, 255);"));
        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(90, 520, 61, 27));
        clearButton->setStyleSheet(QString::fromUtf8("border-image: \\*url();\n"
"color: rgb(0, 255, 255);"));
        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(160, 520, 61, 27));
        deleteButton->setStyleSheet(QString::fromUtf8("border-image: \\*url();\n"
"color: rgb(0, 255, 255);\n"
""));
        previousButton = new QToolButton(centralwidget);
        previousButton->setObjectName(QString::fromUtf8("previousButton"));
        previousButton->setGeometry(QRect(540, 520, 41, 31));
        previousButton->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/previous.png);"));
        playPauseButton = new QToolButton(centralwidget);
        playPauseButton->setObjectName(QString::fromUtf8("playPauseButton"));
        playPauseButton->setGeometry(QRect(590, 520, 41, 31));
        playPauseButton->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/play.png);"));
        nextButton = new QToolButton(centralwidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(640, 520, 41, 31));
        nextButton->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/next.png);"));
        stoptButton = new QToolButton(centralwidget);
        stoptButton->setObjectName(QString::fromUtf8("stoptButton"));
        stoptButton->setGeometry(QRect(690, 520, 41, 31));
        stoptButton->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/stop.png);"));
        volumeButton = new QToolButton(centralwidget);
        volumeButton->setObjectName(QString::fromUtf8("volumeButton"));
        volumeButton->setGeometry(QRect(740, 520, 31, 31));
        volumeButton->setStyleSheet(QString::fromUtf8("border-image: url(:/images_music/volume.png);"));
        volumeSlider = new QSlider(centralwidget);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(790, 530, 71, 21));
        volumeSlider->setStyleSheet(QString::fromUtf8("border-image: \\*url(:/images_music/bk1.png);"));
        volumeSlider->setOrientation(Qt::Horizontal);
        toolButton_6 = new QToolButton(centralwidget);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));
        toolButton_6->setGeometry(QRect(840, 0, 41, 25));
        toolButton_6->setStyleSheet(QString::fromUtf8("border-image: \\*url(:/images_music/bk1.png);\n"
"color: rgb(0, 255, 255);\n"
""));
        toolButton_7 = new QToolButton(centralwidget);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));
        toolButton_7->setGeometry(QRect(800, 0, 41, 25));
        toolButton_7->setStyleSheet(QString::fromUtf8("border-image: \\*url(:/images_music/bk1.png);\n"
"color: rgb(0, 255, 255);"));
        toolButton_8 = new QToolButton(centralwidget);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));
        toolButton_8->setGeometry(QRect(760, 0, 41, 25));
        toolButton_8->setStyleSheet(QString::fromUtf8("border-image: \\*url(:/images_music/bk1.png);\n"
"color: rgb(0, 255, 255);\n"
""));
        songList = new QListWidget(centralwidget);
        songList->setObjectName(QString::fromUtf8("songList"));
        songList->setGeometry(QRect(30, 50, 251, 301));
        songList->setStyleSheet(QString::fromUtf8("border-image: \\*url();\n"
"background-color: rgb(170, 255, 127);\n"
""));
        Music->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Music);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 885, 25));
        Music->setMenuBar(menubar);
        statusbar = new QStatusBar(Music);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Music->setStatusBar(statusbar);

        retranslateUi(Music);

        QMetaObject::connectSlotsByName(Music);
    } // setupUi

    void retranslateUi(QMainWindow *Music)
    {
        Music->setWindowTitle(QApplication::translate("Music", "MainWindow", 0));
        label->setText(QApplication::translate("Music", "\347\210\261\345\245\207\350\211\272\344\270\207\350\203\275\346\222\255\346\224\276\345\231\250", 0));
        pushButton->setText(QApplication::translate("Music", "\346\267\273\345\212\240\346\255\214\346\233\262", 0));
        clearButton->setText(QApplication::translate("Music", "\346\270\205\347\251\272\345\210\227\350\241\250", 0));
        deleteButton->setText(QApplication::translate("Music", "\345\210\240\351\231\244\351\200\211\351\241\271", 0));
        previousButton->setText(QString());
        playPauseButton->setText(QString());
        nextButton->setText(QString());
        stoptButton->setText(QString());
        volumeButton->setText(QString());
        toolButton_6->setText(QApplication::translate("Music", "\345\205\263\351\227\255", 0));
        toolButton_7->setText(QApplication::translate("Music", "\346\234\200\345\244\247\345\214\226", 0));
        toolButton_8->setText(QApplication::translate("Music", "\346\234\200\345\260\217\345\214\226", 0));
    } // retranslateUi

};

namespace Ui {
    class Music: public Ui_Music {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUSIC_H
