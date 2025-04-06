
add_library(addons_bhc OBJECT)
target_link_libraries(addons_bhc PUBLIC addons_int)
set_target_properties(addons_bhc PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_bhc
    PRIVATE
    addons-bhc.h
    addons-bhc.cpp

    bhcoptions.h
    bhctaboptions.h
    bhcoptions.cpp 
    bhctaboptions.cpp

    bhcdialog.h 
    bhcdraw.h
    bhctab.h
    bhcdialog.cpp
    bhcdraw.cpp
    bhctab.cpp
    bhcdialog.ui
    bhctab.ui
)
