# Adapted C Hello World with MRuby

This is from the C API examples provided with the Playdate SDK, with the addition of mruby.



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
