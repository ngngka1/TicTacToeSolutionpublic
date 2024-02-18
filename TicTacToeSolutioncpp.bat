@echo off

dir /b /a-d "cpp.exe" >nul 2>nul
if %errorlevel% neq 0 (
    g++ -O2 .\main_cpp.cpp -o cpp
)

call cpp.exe