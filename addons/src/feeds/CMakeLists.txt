
add_library(addons_feeds OBJECT)
target_link_libraries(addons_feeds PUBLIC addons_int)
set_target_properties(addons_feeds PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_feeds
    PRIVATE
    addons-feeds.h
    addons-feeds.cpp

    feedsoptions.h
    feedsoptions.cpp

    feedsdialog.h
    feedsdialog.cpp
    feedsdialog.ui
)
