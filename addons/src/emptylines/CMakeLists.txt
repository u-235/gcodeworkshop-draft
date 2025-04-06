
add_library(addons_emptylines OBJECT)
target_link_libraries(addons_emptylines PUBLIC addons_int)
set_target_properties(addons_emptylines PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_emptylines
    PRIVATE
    utils-emptylines.h
    utils-emptylines.cpp
)
