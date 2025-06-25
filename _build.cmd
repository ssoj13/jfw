@echo off
cls
rem rd /s /q build
mkdir build
cd build
rem set DESTDIR=C:\Programs\Lib
cmake --log-level ERROR ../ -G "Visual Studio 17 2022" "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x64-windows" && cmake --build . --target ALL_BUILD --config Release
pause
cmake -P cmake_install.cmake