
add_library(gcodeworkshop_int INTERFACE)
target_include_directories(gcodeworkshop_int INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(gcodeworkshop)
target_link_libraries(gcodeworkshop PUBLIC Qt::Core Qt::PrintSupport Qt::Widgets kdiff3 qtsingleapplication addons gcodeshared gcodeworkshop_int  gcodefileserver_int)
set_target_properties(gcodeworkshop PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(gcodeworkshop
    PRIVATE
    include/document.h
    include/documentinfo.h
    include/documentmanager.h
    include/documentstyle.h
    include/documentproducer.h
    include/documentwidgetproperties.h
    include/gcoderdocument.h
    include/gcoderstyle.h
    include/gcoderwidgetproperties.h
    include/gcodeworkshop.h
    include/utils/expressionparser.h
    include/utils/removezeros.h

    src/capslockeventfilter.h
    src/documentwidgetcloseeventfilter.h
    src/findinf.h
    src/gcoder.h
    src/gcodereventfilter.h
    src/gcoderinfo.h
    src/gcoderproducer.h
    src/highlighter.h
    src/highlightmode.h
    src/inlinecalc.h
    src/newfiledialog.h
    src/recentfiles.h
    src/sessiondialog.h
    src/sessionmanager.h
    src/sessionnamedialog.h
    src/setupdialog.h
    src/tooltips.h
    src/capslockeventfilter.cpp
    src/document.cpp
    src/documentinfo.cpp
    src/documentmanager.cpp
    src/documentproducer.cpp
    src/documentstyle.cpp
    src/documentwidgetcloseeventfilter.cpp
    src/documentwidgetproperties.cpp
    src/findinf.cpp
    src/gcoderdocument.cpp
    src/gcodereventfilter.cpp
    src/gcoderinfo.cpp
    src/gcoderproducer.cpp
    src/gcoderstyle.cpp
    src/gcoderwidgetproperties.cpp
    src/gcodeworkshop.cpp
    src/highlighter.cpp
    src/inlinecalc.cpp
    src/main.cpp
    src/newfiledialog.cpp
    src/recentfiles.cpp
    src/sessiondialog.cpp
    src/sessionmanager.cpp
    src/sessionnamedialog.cpp
    src/setupdialog.cpp
    src/utils/expressionparser.cpp
    src/utils/removezeros.cpp
    src/findinfilesdialog.ui
    src/gcodeworkshop.ui
    src/newfiledialog.ui
    src/sessiondialog.ui
    src/sessionnamedialog.ui
    src/setupdialog.ui
)
