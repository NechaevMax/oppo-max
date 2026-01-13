@echo off
REM Скрипт компиляции и запуска с Google Test
chcp 65001 >nul
cd /d "C:\Users\MAX\Desktop\oppo\3 lab"

echo ===== КОМПИЛЯЦИЯ ОСНОВНОЙ ПРОГРАММЫ =====
call vcvars64.bat >nul 2>&1
cl.exe "improved_2_LABA.cpp" /Fe"improved_2_LABA.exe" /W0 /std:c++17
if exist "improved_2_LABA.exe" (
    echo Основная программа скомпилирована успешно.
) else (
    echo Ошибка компиляции основной программы
    pause
    exit /b 1
)

echo.
echo ===== ЗАПУСК ОСНОВНОЙ ПРОГРАММЫ =====
"improved_2_LABA.exe"

echo.
echo Для запуска тестов установите Google Test и скомпилируйте test_2_LABA.cpp
pause
