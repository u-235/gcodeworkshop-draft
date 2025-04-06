
add_library(qtsingleapplication STATIC)
target_include_directories(qtsingleapplication PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(qtsingleapplication PUBLIC Qt::Widgets Qt::Network)
set_target_properties(qtsingleapplication PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(qtsingleapplication
    PRIVATE
    qtlocalpeer.cpp
    qtlocalpeer.h
#   qtlockedfile.cpp
    qtlockedfile.h
#   qtlockedfile_unix.cpp
#   qtlockedfile_win.cpp
    qtsingleapplication.cpp
    qtsingleapplication.h
)
