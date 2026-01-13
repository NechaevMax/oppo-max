@echo off
chcp 65001 >nul 2>&1
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"

echo Инициализирую Visual Studio...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

echo Компилирую improved_2_LABA.cpp...
cl.exe /utf-8 /EHsc improved_2_LABA.cpp

pause
