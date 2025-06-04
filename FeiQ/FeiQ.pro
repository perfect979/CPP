QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatdialog.cpp \
    ckernel.cpp \
    main.cpp \
    feiqdialog.cpp \
    mediator/INetMediator.cpp \
    mediator/UdpMediator.cpp \
    net/UdpNet.cpp

HEADERS += \
    chatdialog.h \
    ckernel.h \
    feiqdialog.h \
    mediator/INetMediator.h \
    mediator/UdpMediator.h \
    net/INet.h \
    net/UdpNet.h \
    net/packDef.h

FORMS += \
    chatdialog.ui \
    feiqdialog.ui

INCLUDEPATH += \
    ./net \
    ./mediator

LIBS += \
    -lWs2_32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
