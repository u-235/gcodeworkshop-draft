
add_library(addons_comment OBJECT)
target_link_libraries(addons_comment PUBLIC addons_int)
set_target_properties(addons_comment PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_comment
    PRIVATE
    utils-comment.h
    utils-comment.cpp
)
