#-------------------------------------------------
#
# Project created by QtCreator 2017-05-20T10:10:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VMR2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphplotter.cpp \
    graphdata.cpp \
    loadfiledialog.cpp \
    fileloader.cpp \
    axisparam.cpp

HEADERS  += mainwindow.h \
    graphplotter.h \
    graphdata.h \
    loadfiledialog.h \
    fileloader.h \
    axisparam.h

FORMS    += mainwindow.ui \
    loadfiledialog.ui
