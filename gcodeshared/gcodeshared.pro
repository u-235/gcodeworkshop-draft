
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
    src/gui/actions/actionkit.cpp \
    src/gui/serialport/serialporttestdialog.cpp \
    src/gui/serialport/serialtransmissiondialog.cpp \
    src/gui/serialport/serialportconfigdialog.cpp

HEADERS += \
    include/gui/actions/actionkit.h \
    include/gui/serialport/serialporttestdialog.h \
    include/gui/serialport/serialtransmissiondialog.h \
    include/gui/serialport/serialportconfigdialog.h \
    include/serialportsettings.h \
    include/version.h

FORMS += \
    src/gui/serialport/serialporttestdialog.ui \
    src/gui/serialport/serialtransmissiondialog.ui \
    src/gui/serialport/serialportconfigdialog.ui


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
