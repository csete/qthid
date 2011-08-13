#-------------------------------------------------
#
# Qthid project file.
#
#-------------------------------------------------

QT       += core gui

TEMPLATE = app

macx {
#    TARGET = "FCD Controller"
    TARGET = Qthid
} else {
    TARGET = qthid
}


# disable debug messages in release
CONFIG(debug, debug|release) {
    # Define version string (see below for releases)
    VER = $$system(git describe --abbrev=8)
} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    VER = 3.1
}

# Tip from: http://www.qtcentre.org/wiki/index.php?title=Version_numbering_using_QMake
VERSTR = '\\"$${VER}\\"'          # place quotes around the version string
DEFINES += VERSION=\"$${VERSTR}\" # create a VERSION macro containing the version string

SOURCES +=\
        mainwindow.cpp \
    main.cpp \
    fcd.c \
    freqctrl.cpp \
    iqbalance.cc

mac: SOURCES += hidmac.c
win32: SOURCES += hidwin.cpp
linux-g++: SOURCES +=hid-libusb.c

HEADERS  += mainwindow.h \
    hidapi.h \
    fcd.h fcdhidcmd.h \
    freqctrl.h \
    iqbalance.h

FORMS    += mainwindow.ui \
    iqbalance.ui

mac:LIBS += /System/Library/Frameworks/CoreFoundation.framework/CoreFoundation \
    /System/Library/Frameworks/IOKit.framework/Versions/A/IOKit
win32:LIBS += "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0\\Lib\\setupapi.lib"

# libusb-1.0 on Linux uses pkg-config
linux-g++ {
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
