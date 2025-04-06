
add_library(gcodefileserver_int INTERFACE)
target_include_directories(gcodefileserver_int INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_executable(gcodefileserver)
target_link_libraries(gcodefileserver
    PUBLIC
    Qt::Core
    Qt::Widgets
    Qt::SerialPort
    gcodefileserver_int
    kdiff3
    qtsingleapplication
    gcodeshared
)
set_target_properties(gcodefileserver PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(gcodefileserver
    PRIVATE
    filechecker.cpp
    filechecker.h
    filechecker.ui
    gcodefileserver.cpp
    gcodefileserver.h
    gcodefileserver.ui
    main.cpp
)
