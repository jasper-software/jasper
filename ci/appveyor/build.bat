cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=c:\tmp\jasper -DALLOW_IN_SOURCE_BUILD=true . || exit /B 1

rem msbuild INSTALL.vcxproj /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" || exit /B 1

msbuild INSTALL.vcxproj /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" || exit /B 1
