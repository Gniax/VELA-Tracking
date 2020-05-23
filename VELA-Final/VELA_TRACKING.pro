#-------------------------------------------------
#
# Project created by QtCreator 2020-03-20T16:20:01
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VELA_TRACKING
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        velawindow.cpp \
    coureur.cpp \
    balise.cpp \
    graphicmanager.cpp \
    testecourse.cpp \
    serverwifi.cpp \
    sqlmanager.cpp \
    clientthreadhandler.cpp \
    optioninformation.cpp \
    optionwindow.cpp

HEADERS += \
        velawindow.h \
    coureur.h \
    balise.h \
    graphicmanager.h \
    testecourse.h \
    serverwifi.h \
    sqlmanager.h \
    clientthreadhandler.h \
    optioninformation.h \
    optionwindow.h

FORMS += \
        velawindow.ui \
    optioninformation.ui \
    optionWindow.ui

unix:!macx: LIBS += -L$$PWD/lib/ -lmysql

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libmysql.a


INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES += \
    ../logo.png


