@echo off

IF NOT DEFINED MSVC (
    echo MSVC is not defined in the environment.
    pause
    exit /b 1
)

CALL "%MSVC%\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

mkdir cmake-build-windows
cd cmake-build-windows
cmake .. --preset=windows-from-windows
pause
