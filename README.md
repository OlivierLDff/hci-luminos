# Luminos App

Luminos app is blabla



## Build

### Windows

### Linux

### MacOs

Install Homebrew & CMake

```sh
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install cmake
```

Export Qt toolchain directory into QT_DIR

```sh
export QT_DIR=/Users/paul/Qt/5.9.2/clang_64/
```

CMake

```Sh
git clone "https://github.com/OlivierLdff/hci-luminos.git"
cd ./hci-luminos/
mkdir build && cd build
cmake -G Xcode -DCMAKE_PREFIX_PATH=$QT_DIR ..
```

### Android

