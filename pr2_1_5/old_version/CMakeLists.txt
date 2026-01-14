cmake_minimum_required(VERSION 3.16)
project(menu_app_old LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(menu_app_old src/main.cpp)
# Подсовываем заглушку windows.h для не-Windows систем
target_include_directories(menu_app_old PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(menu_app_old PRIVATE -Wall -Wextra -Wpedantic)
