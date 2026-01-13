@echo off
chcp 65001 >nul
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"
echo Компилирую C++ код...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl.exe "2 LABA.cpp" /Fe"2 LABA.exe" /W0
if exist "2 LABA.exe" (
    echo.
    echo Запускаю программу...
    echo.
    echo 5 | "2 LABA.exe"
) else (
    echo Ошибка компиляции
)
pause
