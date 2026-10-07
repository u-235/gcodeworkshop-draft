# GCoderFileServer

TEMPLATE = app
TARGET = gcodefileserver

QT *= widgets serialport network
MODULES *= kdiff3 qtsingleapplication GCodeShared

include(../common.pri)

defined(VERSION, var) {
    # Define the version as a string literal
    # https://stackoverflow.com/a/2411008
    DEFINES += 'GCODEWORKSHOP_VERSION=\\"$$VERSION\\"'
} else {
    VERSION = $$getVersion()
}

INCLUDEPATH += include src

SOURCES = \
    src/gcodefileserver.cpp \
    src/gui/filechecker/filechecker.cpp \
    src/gui/fileserver/defaultkeysequences.cpp \
    src/gui/fileserver/knownactions.cpp \
    src/gui/fileserver/mainwindow.cpp \
    src/gui/fileserver/windowactions.cpp \
    src/main.cpp

HEADERS = \
    include/gcodefileserver.h \
    include/gui/fileserver/knownactions.h \
    src/gui/filechecker/filechecker.h \
    src/gui/fileserver/defaultkeysequences.h \
    src/gui/fileserver/mainwindow.h \
    src/gui/fileserver/windowactions.h

FORMS = \
    src/gui/filechecker/filechecker.ui \
    src/gui/fileserver/mainwindow.ui


# target platforms dependencies
#######################################

unix {
    target.path = $${PREFIX}/bin
    INSTALLS += target
}

macx {
}

win32 {
    target.path = $${PREFIX}/bin
    INSTALLS += target
    QMAKE_TARGET_PRODUCT=GCodeFileServer
}

RC_ICONS = images/edytornc.ico images/edytornc48.ico
