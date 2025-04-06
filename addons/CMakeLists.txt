
add_library(addons_int INTERFACE)
target_include_directories(addons_int INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(addons_int INTERFACE Qt::Core Qt::Widgets gcodeshared gcodeworkshop_int)

add_library(addons STATIC)
target_include_directories(addons PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(addons PUBLIC addons_int PRIVATE gcodeworkshop_int)
set_target_properties(addons PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons
    PRIVATE
    include/addons-actions.h
    src/addons-actions.cpp
    src/addons-context.cpp
    src/addons-context.h
)

add_subdirectory(src/bhc)
add_subdirectory(src/blockskip)
add_subdirectory(src/chamfer)
add_subdirectory(src/cleanup)
add_subdirectory(src/comment)
add_subdirectory(src/compilemacro)
add_subdirectory(src/dot)
add_subdirectory(src/emptylines)
add_subdirectory(src/feeds)
add_subdirectory(src/i2m)
add_subdirectory(src/i2mprog)
add_subdirectory(src/renumber)
add_subdirectory(src/spaces)
add_subdirectory(src/swapaxes)
add_subdirectory(src/triangle)

target_link_libraries(addons PRIVATE
    addons_bhc
    addons_blockskip
    addons_chamfer
    addons_cleanup
    addons_comment
    addons_compilemacro
    addons_dot
    addons_emptylines
    addons_feeds
    addons_i2m
    addons_i2mprog
    addons_renumber
    addons_spaces
    addons_swapaxes
    addons_triangle
)
