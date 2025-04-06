
add_library(basic STATIC)
target_include_directories(basic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(basic PUBLIC Qt::Core)
target_sources(basic
    PRIVATE
    basic_interpreter.cpp
    basic_interpreter.h
)
