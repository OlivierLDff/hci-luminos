# argv3 is QT_ANDROID_APP_PATH
set(QT_ANDROID_APP_PATH ${CMAKE_ARGV3})
# argv5 is QT_ANDROID_APP_EXTRA_LIBS
#set(QT_ANDROID_APP_EXTRA_LIBS ${CMAKE_ARGV5})
# oliv : todo when it will be needed ...
configure_file(${CMAKE_CURRENT_BINARY_DIR}/qtdeploy.json.in ${CMAKE_CURRENT_BINARY_DIR}/qtdeploy.json @ONLY)