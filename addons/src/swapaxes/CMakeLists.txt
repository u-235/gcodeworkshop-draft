
add_library(addons_swapaxes OBJECT)
target_link_libraries(addons_swapaxes PUBLIC addons_int)
set_target_properties(addons_swapaxes PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_swapaxes
    PRIVATE
    addons-swapaxes.h
    addons-swapaxes.cpp

    swapaxesoptions.h
    swapaxesoptions.cpp

    swapaxesdialog.h
    swapaxesdialog.cpp
    swapaxesdialog.ui

    utils-swapaxes.h
    utils-swapaxes.cpp
)
