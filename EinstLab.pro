#-------------------------------------------------
#
# Project created by QtCreator 2018-08-16T14:18:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EinstLab
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

DEFINES += QT_DLL QWT_DLL
LIBS += -L"C:\Qt\Qt5.6.1\5.6\mingw49_32\lib" -lqwtd
LIBS += -L"C:\Qt\Qt5.6.1\5.6\mingw49_32\lib" -lqwt
INCLUDEPATH += C:\Qt\Qt5.6.1\5.6\mingw49_32\include\Qwt

RC_ICONS = 28.ico

RESOURCES += \
    image.qrc
