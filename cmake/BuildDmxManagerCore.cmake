add_definitions( -DDMX_MANAGER_CORE )
add_definitions( -DOOBJECT )

set(REPOSITORIES_FILE ${PROJECT_SOURCE_DIR}/cmake/repositories/repositories-online.cmake)
MESSAGE(" > All remote dependencies are coming from file : " ${REPOSITORIES_FILE})
include(${REPOSITORIES_FILE})

#Download DmxManagerCore from github
if (CMAKE_VERSION VERSION_LESS 3.2)
	set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
	set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(./cmake/DownloadProject.cmake)

# https://stackoverflow.com/questions/29892929/variables-set-with-parent-scope-are-empty-in-the-corresponding-child-scope-why
set(DMX_MANAGER_CORE DmxManagerCore )
set(DMX_MANAGER_CORE ${DMX_MANAGER_CORE} PARENT_SCOPE )

set(DMX_MANAGER_CORE_STATIC ON)
set(DMX_MANAGER_CORE_SHARED OFF)

set(BUILD_EXAMPLES OFF)
set(BUILD_TESTS OFF)

MESSAGE(" > Download DmxManagerCore from repository : " ${REPOSITORY_DMX_MANAGER_CORE} " with tag : " ${DMX_MANAGER_CORE_VERSION})

download_project(PROJ               ${DMX_MANAGER_CORE}
	GIT_REPOSITORY     ${REPOSITORY_DMX_MANAGER_CORE}
	GIT_TAG            ${DMX_MANAGER_CORE_VERSION}
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	)

add_subdirectory(${DmxManagerCore_SOURCE_DIR} ${DmxManagerCore_BINARY_DIR})

if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories(${DmxManagerCore_SOURCE_DIR}/include)
endif()

foreach(DmxManagerCoreSource IN ITEMS ${DMX_MANAGER_CORE})
	set_target_properties(${DmxManagerCoreSource} PROPERTIES FOLDER ${DMX_MANAGER_CORE} )
endforeach()