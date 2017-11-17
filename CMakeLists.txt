cmake_minimum_required(VERSION 2.8.12) #minimum version required by Qt (http://doc.qt.io/qt-5/cmake-manual.html)

# cmake -G "Visual Studio 15 2017 Win64" ..
# cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
# cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../qt-android-cmake/toolchain/android.toolchain.cmake" -DCMAKE_MAKE_PROGRAM="C:/Users/olivi/AppData/Local/Android/Sdk/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_NATIVE_API_LEVEL=27 ..
# C:/Users/olivi/AppData/Local/Android/Sdk/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe
# https://github.com/LaurentGomila/qt-android-cmake

#debug : C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe logcat build-debug.apk | grep libChatTutorialCmakeBuild
# C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe install build-debug.apk
# C:/Users/olivi/AppData/Local/Android/Sdk/platform-tools/adb.exe uninstall org.HCI.Luminos

set(UI_PROJECT_NAME Luminos)
set(UI_APP_NAME ${UI_PROJECT_NAME})
set(COMPANY_PROJECT_NAME HCI)

project(${UI_PROJECT_NAME})

# --------------------------------------------------------
# -------------------- QT 5 BUILDING ---------------------
# --------------------------------------------------------

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTORCC ON) # https://cmake.org/cmake/help/v3.0/prop_tgt/AUTORCC.html#prop_tgt:AUTORCC

set (CMAKE_CXX_STANDARD 11) #required by Qt5

# Set cmake prefix path to help search and windows, 
# this is ugly but avoid setting PATH for windows compilation

if(WIN32)
	if(ANDROID) # Arm android toolchain
		set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.2\\5.9.2\\android_armv7\\")
	else(ANDROID)

		if(MSVC) # Microsoft visual Studio toolchain -G "Visual Studio 15 2017 Win64"
			set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.2\\5.9.2\\msvc2017_64\\")
		elseif(CMAKE_COMPILER_IS_GNUCXX) # Mingw32 toolchain -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
			set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.2\\5.9.2\\mingw53_32\\")		
		endif()
	endif(ANDROID)
endif(WIN32)

# Qt5Core :
# QuickControls2 : 
# Quick : 
find_package(Qt5 COMPONENTS Core QuickControls2 Quick REQUIRED)

if(ANDROID) #in android the executable is loaded as a library by the java activity
	add_library(${UI_PROJECT_NAME} SHARED "./src/main.cpp" "./qml/qml.qrc")
else()
	add_executable(${UI_PROJECT_NAME} "./src/main.cpp" "./qml/qml.qrc")
endif()

if(ANDROID)
	include(${PROJECT_SOURCE_DIR}/cmake/AddQtAndroidApk.cmake)
	add_qt_android_apk(${UI_PROJECT_NAME}_apk ${UI_PROJECT_NAME} 
		NAME ${UI_APP_NAME}
		VERSION_CODE 1
		PACKAGE_NAME org.${COMPANY_PROJECT_NAME}.${APP_NAME}
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
		)
elseif(UNIX)
	# Nothing special required
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::QuickControls2 Qt5::Quick)
