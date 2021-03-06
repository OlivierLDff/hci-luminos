# Sets:
#   Qt5_FOUND: TRUE if Qt5 is found.
#   Qt5_INCLUDE_DIRS: Include directories for Qt5.
#   Qt5_LIBRARIES: Libraries required to link Qt5.

find_package(Qt5Core QUIET)

if(Qt5Core_FOUND)
    set(Qt5_FOUND TRUE)
    set(Qt5_INCLUDE_DIRS "${Qt5Core_INCLUDE_DIRS}")
    set(Qt5_LIBRARIES "${Qt5Core_LIBRARIES}")
endif()

if(Qt5_FOUND)
    message(STATUS "Found Qt ${Qt5Core_VERSION_STRING}")
else()
    if(Qt5_FIND_REQUIRED)
        set(Qt5_ERROR_MESSAGE "Could not find Qt5:")
        if(NOT Qt5Core_FOUND)
            set(Qt5_ERROR_MESSAGE "${Qt5_ERROR_MESSAGE} Qt5Core")
        endif()
        set(Qt5_ERROR_MESSAGE "${Qt5_ERROR_MESSAGE} not found, try "
            "setting -DQt5Core_DIR to the directories "
            "containing Qt5CoreConfig.cmake")
        message(FATAL_ERROR ${Qt5_ERROR_MESSAGE})
    endif()
endif()