# Adapted C Hello World with MRuby

This is from the C API examples provided with the Playdate SDK, with the addition of mruby.

## Overview

Parts of the app:

1. src/main.c - the Playdate C SDK entrypoint, this file is responsible for initializing the mruby virtual machine and configuring the C<->Ruby bridge
2. vendor/mruby/mruby.c - the mruby virtual machine, compiled amalgam from the mruby project with alloc_func definitions commented out (still not clear why this is necessary in addition to override - only seems to matter for ARM gcc)
4. src/bindings/rubybind_base.c - entry point for main.c to set up the Ruby virtual machine bindings
5. src/bindings/*.c - Ruby bindings for various Playdate C API modules
6. lib/*.rb - Ruby files that will be compiled into the core binary, loaded after bindings
7. cartridge/game.rb - Core game file, loaded after everything else, hooked into Playdate's C update loop

Note that cartridge is compiled as mrb files and included in the final pdx build for dynamic loading. Theoretically this means games can be recompiled with just an MRBC command, but to get there a lot more bindings / setup to the standard library need done.

### Resources

https://github.com/mruby/mruby/blob/master/doc/guides/capi.md
https://sdk.play.date/3.0.6/Inside%20Playdate%20with%20C.html

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
