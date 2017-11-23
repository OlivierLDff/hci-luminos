# ┌─────────────────────────────────────────────────────────────────────────────────────────────────────────┐
# │																											│
# │												LUMINOS APP 												│
# │																											│
# └─────────────────────────────────────────────────────────────────────────────────────────────────────────┘

# Project for HCI class in BME 2017 - Group 7
# Olivier Le Doeuff CR0ZOQ
# Paul Cales VWTFKW
# Andres Camilo Bernal Moreno
# Antonio Eliseu

# ┌──────────────────────────────────────────────────────────────────┐
# │						  CMAKE PROPERTIES							 │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 2.8.12) #minimum version required by Qt (http://doc.qt.io/qt-5/cmake-manual.html)

# cmake -G "Visual Studio 15 2017 Win64" ..
# cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
# cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain/android.toolchain.cmake" -DCMAKE_MAKE_PROGRAM="C:/Users/olivi/AppData/Local/Android/Sdk/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_NATIVE_API_LEVEL=27 ..
# C:/Users/olivi/AppData/Local/Android/Sdk/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe
# https://github.com/LaurentGomila/qt-android-cmake

#debug : C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe logcat build-debug.apk | grep Luminos
# C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe install build-android-debug.apk
# C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe uninstall org.HCI.LuminosApp

# uninstall and install apk :
# C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe uninstall org.HCI.LuminosApp && C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe install build-android-debug.apk

# ┌──────────────────────────────────────────────────────────────────┐
# │						  PROJECT SETTINGS							 │
# └──────────────────────────────────────────────────────────────────┘

set(UI_PROJECT_NAME Luminos)
set(UI_APP_NAME LuminosApp)
set(COMPANY_PROJECT_NAME HCI)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

########################
# PRE PROJECT SETTINGS #
########################

option(ANDROID_BACK "ANDROID_BACK" off)

option(ANDROID_TOOLCHAIN "ANDROID_TOOLCHAIN" off) #-DANDROID_TOOLCHAIN

if(${ANDROID_TOOLCHAIN}) 
	set(CMAKE_TOOLCHAIN_FILE "./cmake/toolchain/android.toolchain.cmake")
endif(${ANDROID_TOOLCHAIN})

project(${UI_PROJECT_NAME})

#########################
# POST PROJECT SETTINGS #
#########################

option(USE_DMX_BACKEND "USE_DMX_BACKEND" on) #-DUSE_DMX_BACKEND

if(${USE_DMX_BACKEND})
	include(${PROJECT_SOURCE_DIR}/cmake/BuildDmxManagerCore.cmake)
endif(${USE_DMX_BACKEND})


# ┌──────────────────────────────────────────────────────────────────┐
# │						  	QT5 BUILDING							 │
# └──────────────────────────────────────────────────────────────────┘

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTORCC ON) # https://cmake.org/cmake/help/v3.0/prop_tgt/AUTORCC.html#prop_tgt:AUTORCC

set (CMAKE_CXX_STANDARD 11) #required by Qt5

# ┌──────────────────────────────────────────────────────────────────┐
# │					QT_DIR (really ugly right know)					 │
# └──────────────────────────────────────────────────────────────────┘

# Set cmake prefix path to help search and windows, 
# this is ugly but avoid setting PATH for windows compilation

if(ANDROID) # Arm android toolchain
	set (CMAKE_PREFIX_PATH "C:/Qt/Qt5.9.2/5.9.2/android_armv7/")
else(ANDROID)
	if(MSVC) # Microsoft visual Studio toolchain -G "Visual Studio 15 2017 Win64"
		set (CMAKE_PREFIX_PATH "C:/Qt/Qt5.9.2/5.9.2/msvc2017_64/")
	elseif(CMAKE_COMPILER_IS_GNUCXX) # Mingw32 toolchain -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
		set (CMAKE_PREFIX_PATH "C:/Qt/Qt5.9.2/5.9.2/mingw53_32/")		
	endif()
endif(ANDROID)	

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

# ┌──────────────────────────────────────────────────────────────────┐
# │						  FILE DEPENDENCIES							 │
# └──────────────────────────────────────────────────────────────────┘

set(SRC_DIR src)
set(INC_DIR include)
set(QML_DIR qml)
set(IM_DIR images)

set(SRC_ROOT_PATH ${PROJECT_SOURCE_DIR}/${SRC_DIR})
set(INC_ROOT_PATH ${PROJECT_SOURCE_DIR}/${INC_DIR})
set(QML_ROOT_PATH ${PROJECT_SOURCE_DIR}/${QML_DIR})
set(IM_ROOT_PATH ${PROJECT_SOURCE_DIR}/${IM_DIR})

#[[file(
	GLOB_RECURSE  UI_SRC 
	LIST_DIRECTORIES false
	${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/*.c*
)
file(
	GLOB_RECURSE  UI_INC
	LIST_DIRECTORIES false
	${CMAKE_CURRENT_SOURCE_DIR}/${INC_DIR}/*.h*
)]]
set(UI_SRC
	${SRC_ROOT_PATH}/FixturesModel.cpp
	${SRC_ROOT_PATH}/main.cpp
	${SRC_ROOT_PATH}/SensorModel.cpp
	)
set(UI_INC
	${INC_ROOT_PATH}/FixturesModel.hpp
	${INC_ROOT_PATH}/SensorModel.hpp
	)
file(
	GLOB_RECURSE  UI_QML
	LIST_DIRECTORIES false
	${CMAKE_CURRENT_SOURCE_DIR}/${QML_DIR}/*.qml
)

file(
	GLOB UI_RES
	LIST_DIRECTORIES false
	./${QML_DIR}/qml.qrc
	./${IM_DIR}/images.qrc 
)

if(WIN32)
	set(UI_OS_DEPENDENCY ${IM_DIR}/windows/icon.rc)
elseif(APPLE)
	set(UI_OS_DEPENDENCY ${PROJECT_SOURCE_DIR}/images/apple/AppIcon.icns)
elseif(UNIX)
	#todo linux http://doc.qt.io/qt-5/appicon.html
elseif(ANDROID)
	#todo android
elseif(IOS)
	#todo ios
endif()

set(UI_FILE_DEPENDANCIES 
	${UI_SRC} 
	${UI_INC} 
	${UI_QML}
	${UI_RES}
	${UI_OS_DEPENDENCY}
)

## CREATE FOLDER

foreach(_source IN ITEMS ${UI_SRC})
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel ${SRC_ROOT_PATH} "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()

foreach(_source IN ITEMS ${UI_INC})
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel  ${INC_ROOT_PATH} "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()

foreach(_source IN ITEMS ${UI_QML})
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel ${PROJECT_SOURCE_DIR} "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()

source_group(res FILES ${UI_RES})
source_group(os FILES ${UI_OS_DEPENDENCY})

# ┌──────────────────────────────────────────────────────────────────┐
# │						  CREATE EXECUTABLE 						 │
# └──────────────────────────────────────────────────────────────────┘

if(ANDROID AND NOT ${ANDROID_BACK}) #in android the executable is loaded as a library by the java activity
	add_library(${UI_PROJECT_NAME} SHARED ${UI_FILE_DEPENDANCIES})
else()
	add_executable(${UI_PROJECT_NAME} ${UI_FILE_DEPENDANCIES})
endif()

# ┌──────────────────────────────────────────────────────────────────┐
# │						  TARGET PROPERTIES	 						 │
# └──────────────────────────────────────────────────────────────────┘

if(ANDROID AND NOT ${ANDROID_BACK})
	include(${PROJECT_SOURCE_DIR}/cmake/android/AddQtAndroidApk.cmake)
	add_qt_android_apk(${UI_PROJECT_NAME}_apk ${UI_PROJECT_NAME} 
		NAME ${UI_APP_NAME}
		VERSION_CODE 1
		PACKAGE_NAME org.${COMPANY_PROJECT_NAME}.${UI_APP_NAME}
		PACKAGE_SOURCES ${PROJECT_SOURCE_DIR}/android-src/
		BUILDTOOLS_REVISION "27.0.1")
elseif(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(${UI_PROJECT_NAME} PROPERTIES
		WIN32_EXECUTABLE YES
		LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		MACOSX_BUNDLE YES
		MACOSX_BUNDLE_ICON_FILE AppIcon.icns
		)
elseif(UNIX)
	# Nothing special required
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif()

# ┌──────────────────────────────────────────────────────────────────┐
# │						  LINK LIBRARIES 	 						 │
# └──────────────────────────────────────────────────────────────────┘

target_include_directories(${UI_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/${INC_DIR})
target_link_libraries(${UI_PROJECT_NAME} Qt5::Core Qt5::QuickControls2 Qt5::Quick ${DMX_MANAGER_CORE})

# ┌──────────────────────────────────────────────────────────────────┐
# │						  BACKEND SOFTWARE 	 						 │
# └──────────────────────────────────────────────────────────────────┘

set(BACKEND_NAME LuminosBackendApp)

if(WIN32)
	set(BACKEND_OS_DEPENDENCY ${IM_DIR}/windows/iconbackend.rc)
elseif(APPLE)
	set(BACKEND_OS_DEPENDENCY ${PROJECT_SOURCE_DIR}/images/apple/AppIcon.icns)
endif()

set(BACKEND_FILE_DEPENDANCIES
	${PROJECT_SOURCE_DIR}/src/MainBackend.cpp
	${PROJECT_SOURCE_DIR}/include/SensorModel.hpp
	${PROJECT_SOURCE_DIR}/src/SensorModel.cpp
	${UI_QML}
	${UI_RES}
	${BACKEND_OS_DEPENDENCY}
	)

if(ANDROID AND ${ANDROID_BACK}) #in android the executable is loaded as a library by the java activity
	add_library(${BACKEND_NAME} SHARED ${BACKEND_FILE_DEPENDANCIES})
else()
	add_executable(${BACKEND_NAME} ${BACKEND_FILE_DEPENDANCIES})
endif()

if(ANDROID AND ${ANDROID_BACK})
	include(${PROJECT_SOURCE_DIR}/cmake/android/AddQtAndroidApk.cmake)
	add_qt_android_apk(${BACKEND_NAME}_apk ${BACKEND_NAME} 
		NAME ${BACKEND_NAME}
		VERSION_CODE 1
		PACKAGE_NAME org.${COMPANY_PROJECT_NAME}.${BACKEND_NAME}
		PACKAGE_SOURCES ${PROJECT_SOURCE_DIR}/android-backend-src/
		BUILDTOOLS_REVISION "27.0.1")
elseif(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(${BACKEND_NAME} PROPERTIES
		WIN32_EXECUTABLE YES
		LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
	set_target_properties(${BACKEND_NAME} PROPERTIES
		MACOSX_BUNDLE YES
		MACOSX_BUNDLE_ICON_FILE BackendAppIcon.icns
		)
elseif(UNIX)
	# Nothing special required
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif()

if(DUSE_DMX_BACKEND)
	set(OOBJECTLIB OObject)
endif()	

target_include_directories(${BACKEND_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/${INC_DIR} )
target_link_libraries(${BACKEND_NAME} Qt5::Core Qt5::QuickControls2 Qt5::Quick ${OOBJECTLIB})
