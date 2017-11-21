# Luminos App

Luminos app is blabla

## Platforms

* Windows
* Linux
* MacOs
* Android
* iOS (not far)

## Requirement

These are the base requirements to build and use DmxManagerCore

* [CMake](https://cmake.org/download/) - Cmake 2.8.1 or newer
* [Git](https://git-scm.com/) - Git 2.13 or newer
* [Qt5](http://doc.qt.io/qt-5/)
  * Qt5Core
  * Qt5Quick
  * Qt5QuickControls2

## Get the project

```sh
git clone "https://github.com/OlivierLdff/hci-luminos.git"
cd hci-luminos
mkdir build && cd build
```
## Build

### Windows

#### Requirement

Project can be build with 

* [Microsoft Visual Studio 2017](https://www.visualstudio.com/) - Cmake generator will be `-G " Visual Studio 15 2017 Win64"` or `-G " Visual Studio 15 2017 Win32"` depending on toolchain


* [MinGW](http://www.mingw.org/)  - Cmake generator will be `-G "MinGW Makefiles"` *(this will require `mingw32-make` or `mingw64-make`)*

#### How to run CMake

First, you must make sure that the following environment variables are defined:

* `QT_DIR` : root directory of QtToolchain

```sh
export QT_DIR=C:/Qt/Qt5.9.2/5.9.2/msvc2017_64/
export QT_DIR=C:/Qt/Qt5.9.2/5.9.2/msvc2017_32/
export QT_DIR=C:/Qt/Qt5.9.2/5.9.2/mingw53_64/
export QT_DIR=C:/Qt/Qt5.9.2/5.9.2/mingw53_32/
```

You can then run CMake:

```
cmake -G <generator> ..
cmake -G "Visual Studio 15 2017 Win64" ..
cmake -G .. #"Visual Studio 15 2017 Win32" is the default conf on windows
cmake -G "MinGW Makefiles" .. && mingw64-make
cmake -G "MinGW Makefiles" .. && mingw32-make
```

**IMPORTANT**  `QTDIR` need to match generator

#### Debug

```
-DCMAKE_BUILD_TYPE=Debug
```

#### Release

```
-DCMAKE_BUILD_TYPE=Release
```

### Linux

#### Requirement

* todo

#### How to run cmake

* todo

### MacOs

#### How to run CMake

First, you must make sure that the following environment variables are defined:

- `QT_DIR` : root directory of QtToolchain

```sh
export QT_DIR=/Users/paul/Qt/5.9.2/clang_64/
```

You can then run CMake:

```Sh
cmake -G Xcode ..
```

### Android

Build is based on [Qt Android CMake utility](https://github.com/OSSIA/score/tree/master/CMake/Android/qt-android-cmake). See [here](./cmake/android/licence.) for modification.

#### Requirement

* [Android SDK Tools](https://developer.android.com/studio/index.html)
* [The Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)
* [Java SE Development Kit](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html) (JDK) v6 or later. You can also use [OpenJDK](http://openjdk.java.net/) on Linux.
* [Apache Ant](http://ant.apache.org/bindownload.cgi) v1.8 or later

[Qt For Android](http://doc.qt.io/qt-5/androidgs.html) is the reference in everything here.

#### How to run CMake

* `ANDROID_SDK` : todo
* `ANDROID_NDK` : todo
* `JAVA_HOME` : todo

##### Windows

* `ANDROID_MAKE` : 

```
cmake -G "MinGW Makefiles" -DANDROID_TOOLCHAIN=ON -DCMAKE_MAKE_PROGRAM="C:/Users/olivi/AppData/Local/Android/Sdk/ndk-bundle/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_NATIVE_API_LEVEL=27 ..
```

set(DCMAKE_MAKE_PROGRAM ${ANDROID_SDK}/ndk-bundle/prebuilt/windows-x86-64/bin/make.exe)

**IMPORTANT**  `QT_DIR` need to match generator

##### Windows error

todo : if error GNU 9.0.1 is meed comment line xxx in file xxx

## Todo

### Main Control

*[ ] Color Picker Page
     *[ ] Color Picker in QML
     *[ ] Attribute R G B
*[ ] Effect Page
     *[ ] Pad
*[ ] Map Page
     *[ ] Selectable object
     *[ ] Select All button
     *[ ] Clear Selection
*[ ] Settings Page
*[ ] C++ backend
     *[ ] 6 fixture rgb
     *[ ] Artnode Broadcast to 255.255.255.255 universe 1
     *[ ] Wyg plan
     *[ ] Change weather image

