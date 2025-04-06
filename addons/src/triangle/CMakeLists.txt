
add_library(addons_triangle OBJECT)
target_link_libraries(addons_triangle PUBLIC addons_int)
set_target_properties(addons_triangle PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
target_sources(addons_triangle
    PRIVATE
    addons-triangle.h
    addons-triangle.cpp

    triangleoptions.h
    triangleoptions.cpp

    triangledialog.h
    triangledialog.cpp
    triangledialog.ui
)
