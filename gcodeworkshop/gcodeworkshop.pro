
TEMPLATE = app
TARGET = gcodeworkshop

QT *= widgets printsupport serialport network
MODULES *= kdiff3 qtsingleapplication GCodeShared GCodeFileServer addons

include(../common.pri)

defined(VERSION, var) {
    # Define the version as a string literal
    # https://stackoverflow.com/a/2411008
    DEFINES += 'GCODEWORKSHOP_VERSION=\\"$$VERSION\\"'
} else {
    VERSION = $$getVersion()
}

INCLUDEPATH += include src


# GCoderWorkShop : public API
#######################################

HEADERS += \
    include/document.h \
    include/documentinfo.h \
    include/documentmanager.h \
    include/documentstyle.h \
    include/documentproducer.h \
    include/documentwidgetproperties.h \
    include/gcoderdocument.h \
    include/gcoderstyle.h \
    include/gcoderwidgetproperties.h \
    include/gcodeworkshop.h \
    include/gui/workshop/knownactions.h \
    include/utils/expressionparser.h \
    include/utils/removezeros.h


# Private headers
#######################################

HEADERS += \
    src/capslockeventfilter.h \
    src/documentwidgetcloseeventfilter.h \
    src/gcoder.h \
    src/gcodereventfilter.h \
    src/gcoderinfo.h \
    src/gcoderproducer.h \
    src/gui/clipboard/clipboardpanel.h \
    src/gui/filebrowser/filebrowserpanel.h \
    src/gui/filetable/filetablepanel.h \
    src/gui/findinfiles/findinfilespanel.h \
    src/gui/findtoolbar/findtoolbar.h \
    src/gui/findtoolbar/findtoolbaractions.h \
    src/gui/newfile/newfiledialog.h \
    src/gui/project/projectpanel.h \
    src/gui/serialport/serialtoolbar.h \
    src/gui/serialport/serialtoolbaractions.h \
    src/gui/session/sessiondialog.h \
    src/gui/session/sessionnamedialog.h \
    src/gui/setup/setupdialog.h \
    src/gui/workshop/defaultkeysequences.h \
    src/gui/workshop/editactions.h \
    src/gui/workshop/fileactions.h \
    src/gui/workshop/helpactions.h \
    src/gui/workshop/statusbaractions.h \
    src/gui/workshop/toolactions.h \
    src/gui/workshop/windowactions.h \
    src/highlighter.h \
    src/highlightmode.h \
    src/inlinecalc.h \
    src/recentfiles.h \
    src/sessionmanager.h \
    src/tooltips.h

SOURCES += \
    src/capslockeventfilter.cpp \
    src/document.cpp \
    src/documentinfo.cpp \
    src/documentmanager.cpp \
    src/documentproducer.cpp \
    src/documentstyle.cpp \
    src/documentwidgetcloseeventfilter.cpp \
    src/documentwidgetproperties.cpp \
    src/gcoderdocument.cpp \
    src/gcodereventfilter.cpp \
    src/gcoderinfo.cpp \
    src/gcoderproducer.cpp \
    src/gcoderstyle.cpp \
    src/gcoderwidgetproperties.cpp \
    src/gcodeworkshop.cpp \
    src/gui/clipboard/clipboardpanel.cpp \
    src/gui/filebrowser/filebrowserpanel.cpp \
    src/gui/filetable/filetablepanel.cpp \
    src/gui/findinfiles/findinfilespanel.cpp \
    src/gui/findtoolbar/findtoolbar.cpp \
    src/gui/findtoolbar/findtoolbaractions.cpp \
    src/gui/newfile/newfiledialog.cpp \
    src/gui/project/projectpanel.cpp \
    src/gui/serialport/serialtoolbar.cpp \
    src/gui/serialport/serialtoolbaractions.cpp \
    src/gui/session/sessiondialog.cpp \
    src/gui/session/sessionnamedialog.cpp \
    src/gui/setup/setupdialog.cpp \
    src/gui/workshop/defaultkeysequences.cpp \
    src/gui/workshop/editactions.cpp \
    src/gui/workshop/fileactions.cpp \
    src/gui/workshop/helpactions.cpp \
    src/gui/workshop/knownactions.cpp \
    src/gui/workshop/statusbaractions.cpp \
    src/gui/workshop/toolactions.cpp \
    src/gui/workshop/windowactions.cpp \
    src/highlighter.cpp \
    src/inlinecalc.cpp \
    src/main.cpp \
    src/recentfiles.cpp \
    src/sessionmanager.cpp \
    src/utils/expressionparser.cpp \
    src/utils/removezeros.cpp

FORMS += \
    src/gcodeworkshop.ui \
    src/gui/clipboard/clipboardpanel.ui \
    src/gui/filebrowser/filebrowserpanel.ui \
    src/gui/findinfiles/findinfilespanel.ui \
    src/gui/newfile/newfiledialog.ui \
    src/gui/project/projectpanel.ui \
    src/gui/session/sessiondialog.ui \
    src/gui/session/sessionnamedialog.ui \
    src/gui/setup/setupdialog.ui


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
    QMAKE_TARGET_PRODUCT=GCodeWorkShop
}

RC_ICONS = images/edytornc.ico images/edytornc48.ico images/ncfile.ico
