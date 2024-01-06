QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/3rdparty/vlc-3.0.20/include
LIBS += -L$$PWD/3rdparty/vlc-3.0.20/bin
LIBS += -lvlccore -lnpvlc -laxvlc -lvlc
SOURCES += \
    filemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    qvlc.cpp \
    qvlccore.cpp \
    qvlcplayer.cpp \
    repeatmode.cpp

HEADERS += \
    filemanager.h \
    mainwindow.h \
    qvlc.h \
    qvlccore.h \
    qvlcplayer.h \
    repeatmode.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QMAKE_EXTENSION_SHOWN = .mp4

RESOURCES += \
    My_Resources.qrc

RC_ICONS= icon.ico
