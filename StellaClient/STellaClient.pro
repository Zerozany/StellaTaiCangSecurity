INCLUDEPATH += D:/boost_1_83_0/include/boost-1_83
LIBS += -LD:/boost_1_83_0/lib
LIBS += -lboost_system-mgw14-mt-d-x64-1_83

INCLUDEPATH += D:\OpenCV\install\include
LIBS += -LD:\OpenCV\install\x64\mingw\bin -lopencv_world480

LIBS += -lws2_32

QT += core gui
QT += core network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConnectSlots.cpp \
    StellaSerialize.cpp \
    init_ui.cpp \
    main.cpp \
    tcpclient.cpp \
    trends_ui.cpp \
    widget.cpp \
    widget2paint.cpp

HEADERS += \
    DataStructure.h \
    StellaSerialize.h \
    tcpclient.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc

DISTFILES +=
