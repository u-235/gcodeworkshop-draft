
add_library(addons_i2m OBJECT)
target_link_libraries(addons_i2m PUBLIC addons_int)
set_target_properties(addons_i2m PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_i2m
    PRIVATE
    addons-i2m.h
    addons-i2m.cpp

    i2moptions.h
    i2moptions.cpp

    i2mdialog.h
    i2mdialog.cpp
    i2mdialog.ui
)
