
VERSION = 2018.07
TEMPLATE = lib
TARGET = gcodeshared
CONFIG += staticlib

QT *= widgets serialport network
# QTextCodec in Qt6
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

include(../common.pri)

INCLUDEPATH += include

# GCoderShared

SOURCES += \
    serialporttestdialog.cpp \
    serialtransmissiondialog.cpp \
    serialportconfigdialog.cpp \
    src/gui/actions/actionkit.cpp

HEADERS += \
    include/gui/actions/actionkit.h \
    include/version.h \
    serialportsettings.h \
    serialporttestdialog.h \
    serialtransmissiondialog.h \
    serialportconfigdialog.h \
    serialportsettings.h

FORMS += \
    serialporttestdialog.ui \
    serialtransmissiondialog.ui \
    serialportconfigdialog.ui


# utils
#######################################

HEADERS += include/gui/longjobhelper.h \
    include/utils/configpage.h \
    include/utils/configdialog.h \
    include/utils/gcode-converter.h \
    include/utils/keysequencemap.h \
    include/utils/medium.h \
    include/utils/guessfilename.h \
    include/utils/splitfile.h \
    src/utils/filepatterns.h

SOURCES += src/gui/longjobhelper.cpp \
    src/utils/configpage.cpp \
    src/utils/configdialog.cpp \
    src/utils/gcode-converter.cpp \
    src/utils/keysequencemap.cpp \
    src/utils/medium.cpp \
    src/utils/guessfilename.cpp \
    src/utils/splitfile.cpp

FORMS += src/utils/configdialog.ui


# resources
#######################################

RESOURCES = application.qrc
