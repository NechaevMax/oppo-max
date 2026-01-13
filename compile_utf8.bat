@echo off
chcp 65001 >nul
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"
echo Компилирую C++ код с UTF-8...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl.exe "2 LABA.cpp" /utf-8 /Fe"2 LABA.exe" /W0
if exist "2 LABA.exe" (
    echo.
    echo Запускаю программу...
    echo.
    "2 LABA.exe"
) else (
    echo Ошибка компиляции
)
pause
