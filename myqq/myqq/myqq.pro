# -------------------------------------------------
# Project created by QtCreator 2016-12-26T22:37:08
# -------------------------------------------------
TARGET = myqq
TEMPLATE = app
SOURCES += main.cpp \
    widget.cpp \
    toolbox1.cpp \
    logindlg.cpp \
    Music.cpp \
    cameramainwin.cpp
HEADERS += widget.h \
    toolbox1.h \
    logindlg.h \
    Music.h \
    cameramainwin.h
RESOURCES += myqq.qrc \
    music.qrc
QT += network
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
QT += core \
    gui multimedia multimediawidgets
OTHER_FILES += 
FORMS += Music.ui
RC_FILE = myapp.rc
