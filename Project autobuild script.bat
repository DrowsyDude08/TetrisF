@echo off
set BUILD_DIR=Build

if not exist "%BUILD_DIR%" (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

set VCPKG_ROOT=D:/vcpkg

cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake ..\

cmake --build . --config Release

pause
