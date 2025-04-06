
add_library(addons_dot OBJECT)
target_link_libraries(addons_dot PUBLIC addons_int)
set_target_properties(addons_dot PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_dot
    PRIVATE
    addons-dot.h
    addons-dot.cpp

    dotoptions.h
    dotoptions.cpp

    dotdialog.h
    dotdialog.cpp
    dotdialog.ui

    utils-insertdot.h
    utils-insertdot.cpp
)
