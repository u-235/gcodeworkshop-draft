
add_library(addons_cleanup OBJECT)
target_link_libraries(addons_cleanup PUBLIC addons_int)
set_target_properties(addons_cleanup PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_cleanup
    PRIVATE
    addons-cleanup.h
    addons-cleanup.cpp

    cleanupoptions.h
    cleanupoptions.cpp

    cleanupdialog.h
    cleanupdialog.cpp
    cleanupdialog.ui

    utils-removebyregex.h
    utils-removebyregex.cpp
)
