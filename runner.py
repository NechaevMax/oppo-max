@echo off
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl.exe 2 LABA.cpp /Fe2 LABA.exe
if exist "2 LABA.exe" (
    echo 5 | "2 LABA.exe"
)
pause
