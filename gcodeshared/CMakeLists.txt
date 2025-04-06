
add_library(gcodeshared STATIC)
target_include_directories(gcodeshared PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(gcodeshared PUBLIC Qt::Core Qt::SerialPort Qt::Widgets)
set_target_properties(gcodeshared PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(gcodeshared
    PRIVATE
    include/version.h
    serialporttestdialog.cpp
    serialtransmissiondialog.cpp
    serialportconfigdialog.cpp
    serialportsettings.h
    serialporttestdialog.h
    serialtransmissiondialog.h
    serialportconfigdialog.h
    serialportsettings.h
    serialporttestdialog.ui
    serialtransmissiondialog.ui
    serialportconfigdialog.ui
    include/ui/longjobhelper.h
    include/utils/medium.h
    include/utils/configpage.h
    include/utils/configdialog.h
    include/utils/gcode-converter.h
    include/utils/guessfilename.h
    include/utils/splitfile.h
    utils/filepatterns.h
    ui/longjobhelper.cpp
    utils/medium.cpp
    utils/configpage.cpp
    utils/configdialog.cpp
    utils/gcode-converter.cpp
    utils/guessfilename.cpp
    utils/splitfile.cpp
    utils/configdialog.ui
    application.qrc
)
