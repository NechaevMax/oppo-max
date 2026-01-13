@echo off
chcp 65001 >nul 2>&1
setlocal enabledelayedexpansion

REM Инициализируем переменные среды Visual Studio
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

echo.
echo ===== ПЕРЕКОМПИЛЯЦИЯ ВСЕХ ФАЙЛОВ =====
echo.

REM Компилирование 2 LABA.cpp
echo Компилирую 2 LABA.cpp...
cl.exe /utf-8 /EHsc "2 LABA.cpp" >nul 2>&1
if errorlevel 1 (
    echo ✗ Ошибка компиляции 2 LABA.cpp
    exit /b 1
) else (
    echo ✓ 2 LABA.cpp скомпилирован успешно
)

REM Компилирование improved_2_LABA.cpp
echo Компилирую improved_2_LABA.cpp...
cl.exe /utf-8 /EHsc improved_2_LABA.cpp 2>&1 | find "error" >nul 2>&1
if errorlevel 1 (
    echo ✓ improved_2_LABA.cpp скомпилирован успешно
) else (
    echo ✗ Ошибка компиляции improved_2_LABA.cpp
    cl.exe /utf-8 /EHsc improved_2_LABA.cpp 2>&1
    exit /b 1
)

REM Компилирование menu_with_tests.cpp
echo Компилирую menu_with_tests.cpp...
cl.exe /utf-8 /EHsc menu_with_tests.cpp >nul 2>&1
if errorlevel 1 (
    echo ✗ Ошибка компиляции menu_with_tests.cpp
    exit /b 1
) else (
    echo ✓ menu_with_tests.cpp скомпилирован успешно
)

echo.
echo ===== ТЕСТИРОВАНИЕ ПРОГРАММ =====
echo.

REM Тест 1: improved_2_LABA
echo Тест improved_2_LABA.exe (опции 1 и 5):
(echo 1& echo 5) | improved_2_LABA.exe
echo.

REM Тест 2: menu_with_tests
echo Тест menu_with_tests.exe (тесты):
menu_with_tests.exe | findstr /C:"РЕЗУЛЬТАТЫ"
echo.

echo ===== КОМПИЛЯЦИЯ УСПЕШНА =====
pause
