cmake_minimum_required(VERSION 2.6)

project(nadir)

find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

MESSAGE(STATUS "Looking for libasound2")
find_library(ASOUND2_LIBRARY asound)
if (ASOUND2_LIBRARY)
    MESSAGE(STATUS "Looking for libasound2 - found")
else (ASOUND2_LIBRARY)
    MESSAGE(FATAL_ERROR "Looking for libasound2 - not found.")
endif (ASOUND2_LIBRARY)

MESSAGE(STATUS "Looking for libjack")
find_library(JACK_LIBRARY jack)
if (JACK_LIBRARY)
    MESSAGE(STATUS "Looking for libjack - found")
else (JACK_LIBRARY)
    MESSAGE(FATAL_ERROR "Looking for libjack - not found.")
endif (JACK_LIBRARY)

MESSAGE(STATUS "Looking for X11")
find_library(X11_LIBRARY X11)
if (X11_LIBRARY)
    MESSAGE(STATUS "Looking for X11 - found")
else (X11_LIBRARY)
    MESSAGE(FATAL_ERROR "Looking for X11 - not found.")
endif (X11_LIBRARY)

MESSAGE(STATUS "Looking for Xtst")
find_library(XTST_LIBRARY Xtst)
if (XTST_LIBRARY)
    MESSAGE(STATUS "Looking for Xtst - found")
else (XTST_LIBRARY)
    MESSAGE(FATAL_ERROR "Looking for Xtst - not found.")
endif (XTST_LIBRARY)

# Source code and images
add_subdirectory(src)
add_subdirectory(src/images)

set(DESKTOP_ENTRY nadir.desktop)
set(APP_ICON nadir.png)

install(FILES ${CMAKE_SOURCE_DIR}/${APP_ICON} DESTINATION share/icons)
install(FILES ${CMAKE_SOURCE_DIR}/${DESKTOP_ENTRY} DESTINATION share/applications)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
