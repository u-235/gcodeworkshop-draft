
add_library(addons_chamfer OBJECT)
target_link_libraries(addons_chamfer PUBLIC addons_int)
set_target_properties(addons_chamfer PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_chamfer
    PRIVATE
    addons-chamfer.h
    addons-chamfer.cpp

    chamferoptions.h
    chamferoptions.cpp

    chamferdialog.h
    chamferdialog.cpp
    chamferdialog.ui
)
