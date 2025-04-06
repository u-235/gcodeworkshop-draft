
add_library(addons_renumber OBJECT)
target_link_libraries(addons_renumber PUBLIC addons_int)
set_target_properties(addons_renumber PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_renumber
    PRIVATE
    addons-renumber.h
    addons-renumber.cpp

    renumberoptions.h
    renumberoptions.cpp

    renumberdialog.h
    renumberdialog.cpp
    renumberdialog.ui

    utils-renumber.h
    utils-renumber.cpp
)
