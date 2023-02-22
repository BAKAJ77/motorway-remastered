@echo off
:start
set /p compilerID="Enter the action to generate project files for (Type 'h' to see supported actions): "

set generatorID=""
if "%compilerID%"=="vs2022" set generatorID="Visual Studio 17 2022"
if "%compilerID%"=="vs2019" set generatorID="Visual Studio 16 2019"

if "%compilerID%"=="h" (
    echo ------------------------------------------------
    echo The available actions are:
    echo 'vs2022' for Visual Studio 17 2022
    echo 'vs2019' for Visual Studio 16 2019
    echo ------------------------------------------------
    goto start
)

if %generatorID%=="" (
    echo The entered action is not supported.
    goto start
)

cd /d "%~dp0\bin\win64\premake"
premake5 %compilerID% --file="..\..\..\premake.lua"

cd /d "%~dp0\libs\glfw"
cmake -B build -D USE_MSVC_RUNTIME_LIBRARY_DLL=false -A x64 -G %generatorID%

pause