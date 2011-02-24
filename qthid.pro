#-------------------------------------------------
#
# Project created by QtCreator 2010-11-30T19:14:15
#
#-------------------------------------------------

QT       += core gui

TARGET = qthid
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    main.cpp \
    fcd.c

mac: SOURCES += hidmac.c
win32: SOURCES += hidwin.cpp
linux-g++: SOURCES +=hid-libusb.c

HEADERS  += mainwindow.h \
    hidapi.h \
    fcd.h

FORMS    += mainwindow.ui

mac:LIBS += /System/Library/Frameworks/CoreFoundation.framework/CoreFoundation \
    /System/Library/Frameworks/IOKit.framework/Versions/A/IOKit
win32:LIBS += "C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Lib\\setupapi.lib"

# libusb-1.0 on Linux uses pkg-config
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libusb-1.0
}

RESOURCES += \
    qthid.qrc


win32 {
    # application icon on Windows
    RC_FILE = qthid.rc
} else:macx {
    # app icon on OSX
    ICON = images/qthid.icns
}

OTHER_FILES += AUTHORS.txt LICENSE.txt NEWS.txt README.txt \
    qthid.rc \
    images/qthid.ico \
    images/qthid.icns

