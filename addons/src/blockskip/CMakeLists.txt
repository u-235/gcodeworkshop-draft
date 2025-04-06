
add_library(addons_blockskip OBJECT)
target_link_libraries(addons_blockskip PUBLIC addons_int)
set_target_properties(addons_blockskip PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_blockskip
    PRIVATE
    utils-blockskip.h
    utils-blockskip.cpp
)
