# argv3 is QT_ANDROID_APP_PATH
set(QT_ANDROID_APP_PATH ${CMAKE_ARGV3})
#[[# argv4 is ARG_DEPENDS
set(ARG_DEPENDS ${CMAKE_ARGV4})

# set the list of dependant libraries
if(ARG_DEPENDS)
	foreach(LIB ${ARG_DEPENDS})
		if(TARGET ${LIB})
			# item is a CMake target, extract the library path
			if(CMAKE_BUILD_TYPE STREQUAL "Debug")
				get_property(LIB_PATH TARGET ${LIB} PROPERTY DEBUG_LOCATION)
			else()
				get_property(LIB_PATH TARGET ${LIB} PROPERTY LOCATION)
			endif()

			message(-> ${LIB_PATH})
			set(LIB ${LIB_PATH})
		endif()
		if(EXTRA_LIBS)
			set(EXTRA_LIBS "${EXTRA_LIBS},${LIB}")
		else()
			set(EXTRA_LIBS "${LIB}")
		endif()
	endforeach()
	set(QT_ANDROID_APP_EXTRA_LIBS "\"android-extra-libs\": \"${EXTRA_LIBS}\",")
endif()]]

configure_file(${CMAKE_CURRENT_BINARY_DIR}/qtdeploy.json.in ${CMAKE_CURRENT_BINARY_DIR}/qtdeploy.json @ONLY)