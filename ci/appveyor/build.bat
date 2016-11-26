echo on

set install_prefix=c:\tmp\jasper\install
set build_dir=c:\tmp\jasper\build
set source_dir=%APPVEYOR_BUILD_FOLDER%

cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%install_prefix% -DALLOW_IN_SOURCE_BUILD=true %source_dir% || exit /B 1

msbuild %source_dir%\INSTALL.vcxproj || exit /B 1

rem msbuild INSTALL.vcxproj /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" || exit /B 1
