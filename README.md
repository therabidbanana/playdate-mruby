# Adapted C Hello World with MRuby

This is from the C API examples provided with the Playdate SDK, with the addition of mruby.

## Overview

Parts of the app:

1. src/main.c - the Playdate C SDK entrypoint, this file is responsible for initializing the mruby virtual machine and configuring the C<->Ruby bridge
2. vendor/mruby/mruby.c - the mruby virtual machine, compiled amalgam from the mruby project with alloc_func definitions commented out
3. src/game.rb - Ruby source

## Prerequisites

mruby binaries must be installed and available on your path - mrbc specifically is used to compile existing .rb files to .c files to be included as bytecode into the build

## Windows build

must have arm toolchain and Visual Studio build tools installed, must start in Visual Studio command prompt

Must also have mrbc (mruby compiler) built and installed (default build location on windows is weirdly \usr\local\bin)

```powershell
$env:PATH+=";C:\Program Files (x86)\Arm\GNU Toolchain mingw-w64-i686-arm-none-eabi\bin\"
$env:PATH+=";C:\usr\local\bin"
```

### Dev build


```
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
```

### Release build

https://sdk.play.date/3.0.6/Inside%20Playdate%20with%20C.html#_building_for_the_playdate_using_nmake

```
mkdir build
cd build
cmake .. -G "NMake Makefiles" --toolchain=~\Documents\PlaydateSDK\C_API\buildsupport\arm.cmake
nmake
```
