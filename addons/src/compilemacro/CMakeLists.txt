
add_library(addons_compilemacro OBJECT)
target_link_libraries(addons_compilemacro PUBLIC addons_int basic)
set_target_properties(addons_compilemacro PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_compilemacro
    PRIVATE
    utils-compilemacro.h
    utils-compilemacro.cpp
)
