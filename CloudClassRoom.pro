#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T10:46:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = CloudClassRoom_Student
TEMPLATE = app

SOURCES += main.cpp\
    classmould.cpp \
    netconfig.cpp \
    globalclass.cpp \
    loadingframe.cpp \
    activemq.cpp \
    process.cpp \
    log.cpp \
    listbuf.cpp \
    ryanConfig.cpp \
    myjson.cpp \
    myhttp.cpp \
    interface.cpp \
    setroomseatdialog.cpp \
    mydialog.cpp \
    message.cpp \
    testnet.cpp \
    waitstu.cpp \
    waitstu2.cpp \
    myqueue.cpp \
    classnameconfig.cpp \
    qthread.cpp \
    loginwidget.cpp \
    waitstudialog.cpp \
    setform.cpp \
    passwordui.cpp \
    stwidget.cpp \
    aboutui.cpp \
    mqmsgprocess.cpp \
    multimsg.cpp

HEADERS  += loginwidget.h \
    setform.h \
    classmould.h \
    netconfig.h \
    loadingframe.h \
    include.h \
    activemq.h \
    process.h \
    log.h \
    listbuf.h \
    ryanConfig.h \
    myjson.h \
    myhttp.h \
    interface.h \
    setroomseatdialog.h \
    mydialog.h \
    global.h \
    testnet.h \
    waitstu.h \
    waitstu2.h \
    myqueue.h \
    classnameconfig.h \
    qthread.h \
    waitstudialog.h \
    buildtime.h \
    passwordui.h \
    stwidget.h \
    aboutui.h \
    mqmsgprocess.h \
    type.h \
    multimsg.h

FORMS    += loginwidget.ui \
    setform.ui \
    loadingframe.ui \
    setroomseatdialog.ui \
    mydialog.ui \
    waitstu.ui \
    waitstu2.ui \
    waitstudialog.ui \
    passwordui.ui \
    stwidget.ui \
    aboutui.ui

LIBS += -lpthread

INCLUDEPATH += /opt/activemq-src/jsoncpp-src-0.5.0/include \
               /opt/activemq-src/apr-1.5.2/include \
               /opt/activemq-src/openssl-1.0.0a/include \
               /opt/activemq-src/activemq-cpp-library-3.9.3/src/main

RESOURCES += \
    resource.qrc

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -lapr-1

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -lcppunit

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -lcrypto

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -lssl

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -lactivemq-cpp

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm

unix:!macx: LIBS += -L$$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm/lib/ -ljson_linux-gcc-5.4.0_libmt

INCLUDEPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
DEPENDPATH += $$PWD/../../../../opt/Trolltech/QtEmbedded-5.7.1-arm
