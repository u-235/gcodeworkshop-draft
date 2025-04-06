
add_library(addons_i2mprog OBJECT)
target_link_libraries(addons_i2mprog PUBLIC addons_int)
set_target_properties(addons_i2mprog PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_i2mprog
    PRIVATE
    addons-i2mprog.h
    addons-i2mprog.cpp

    i2mprogoptions.h
    i2mprogoptions.cpp

    i2mprogdialog.h
    i2mprogdialog.cpp
    i2mprogdialog.ui

    utils-i2mprog.h
    utils-i2mprog.cpp
)
