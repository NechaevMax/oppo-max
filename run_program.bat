@echo off
REM Скрипт компиляции и запуска улучшенной программы
chcp 65001 >nul
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"

echo ===== КОМПИЛЯЦИЯ ОСНОВНОЙ ПРОГРАММЫ =====
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist "%VCVARS%" (
    call "%VCVARS%" >nul 2>&1
) else (
    echo Ошибка: Visual Studio не найден
    echo Проверьте путь к Visual Studio
    pause
    exit /b 1
)

cl.exe "improved_2_LABA.cpp" /Fe"improved_2_LABA.exe" /W0 /std:c++17
if exist "improved_2_LABA.exe" (
    echo.
    echo Основная программа скомпилирована успешно.
    echo.
    echo ===== ЗАПУСК ОСНОВНОЙ ПРОГРАММЫ =====
    echo.
    "improved_2_LABA.exe"
) else (
    echo Ошибка компиляции основной программы
    pause
    exit /b 1
)

echo.
echo Для запуска тестов установите Google Test и скомпилируйте test_2_LABA.cpp
pause
