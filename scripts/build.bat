@echo off
REM Build script for Riley Corpbrain (Windows)
REM Requires Qt6, CMake, Python 3.11 installed and in PATH
cmake -S . -B build
cmake --build build --config Release
REM Optionally, copy corpbrain.sqlite and assets to build output
