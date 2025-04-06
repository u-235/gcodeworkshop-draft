
cmake_minimum_required(VERSION 3.13)

# Project info
project(GCodeWorkShop LANGUAGES CXX)

# Init cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Prepare to find Qt modules.
set(APP_QT_MIN_VERSION 5.0)
set(APP_QT_MODULES Core Network PrintSupport SerialPort Widgets)

set(APP_QT_VERSION ${APP_QT_MIN_VERSION} CACHE STRING "Preferred Qt version")

# What version of Qt are we working with?
if ($CACHE{APP_QT_VERSION} VERSION_LESS 6)
    find_package(QT $CACHE{APP_QT_VERSION} NAMES Qt5)
endif ()

if (NOT QT_FOUND)
    find_package(QT $CACHE{APP_QT_VERSION} NAMES Qt5 Qt6 REQUIRED)
endif ()


set(QT Qt${QT_VERSION_MAJOR})
# Since Qt 6 QTextCodec moved in the Qt 5 Core Compat module.
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    list(APPEND APP_QT_MODULES Core5Compat)
endif ()

# Now we know all the required Qt modules.
find_package(${QT} REQUIRED ${APP_QT_MODULES})

include(cmake/qt-aliases.cmake)
#include(cmake/install-helper.cmake)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 11)

# Run in those subdirectories
add_subdirectory(3rdparty)
add_subdirectory(addons)
add_subdirectory(gcodefileserver)
add_subdirectory(gcodeshared)
add_subdirectory(gcodeworkshop)
#add_subdirectory(lang)
