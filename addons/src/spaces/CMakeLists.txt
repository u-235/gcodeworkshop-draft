
add_library(addons_spaces OBJECT)
target_link_libraries(addons_spaces PUBLIC addons_int)
set_target_properties(addons_spaces PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_spaces
    PRIVATE
    utils-spaces.h
    utils-spaces.cpp
)
