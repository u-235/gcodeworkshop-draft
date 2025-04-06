
add_library(kdiff3 STATIC)
target_include_directories(kdiff3 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(kdiff3 PUBLIC Qt::Core Qt::Widgets)
set_target_properties(kdiff3 PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(kdiff3
    PRIVATE
    common.cpp
    common.h
    compatibility.h
    diff.cpp
    diff.h
    difftextwindow.cpp
    difftextwindow.h
    fileaccess.cpp
    fileaccess.h
    gnudiff_analyze.cpp
    gnudiff_diff.h
    gnudiff_io.cpp
    gnudiff_system.h
    gnudiff_xmalloc.cpp
    kdiff3.cpp
    kdiff3.h
    merger.cpp
    merger.h
    mergeresultwindow.cpp
    mergeresultwindow.h
    optiondialog.cpp
    optiondialog.h
    pdiff.cpp
)

set_property(SOURCE optiondialog.cpp PROPERTY SKIP_AUTOMOC ON)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(kdiff3 PUBLIC Qt::Core5Compat)
endif ()
