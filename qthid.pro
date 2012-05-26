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
    VER = 0.0
}

# Tip from: http://www.qtcentre.org/wiki/index.php?title=Version_numbering_using_QMake
VERSTR = '\\"$${VER}\\"'          # place quotes around the version string
DEFINES += VERSION=\"$${VERSTR}\" # create a VERSION macro containing the version string

SOURCES +=\
    mainwindow.cpp \
    main.cpp \
    freqctrl.cpp \
    iqbalance.cc \
    fcddiagram.cpp \
    firmware.cpp \
    dockifgain.cpp


HEADERS  += \
    mainwindow.h \
    freqctrl.h \
    iqbalance.h \
    fcddiagram.h \
    firmware.h \
    dockifgain.h

FORMS    += \
    iqbalance.ui \
    mainwindow.ui \
    fcddiagram.ui \
    firmware.ui \
    dockifgain.ui

# libfcd on Linux uses pkg-config
## FIXME for Mac and Windows
linux-g++|linux-g++-64 {
    CONFIG += link_pkgconfig
    PKGCONFIG += libfcd
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
