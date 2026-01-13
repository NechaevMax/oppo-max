cmake_minimum_required(VERSION 3.14)
project(MenuManager_Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Основная программа (без main)
add_library(menu_lib OBJECT
    improved_2_LABA.cpp
)
target_compile_options(menu_lib PRIVATE /W4)

# Тесты
enable_testing()
add_executable(menu_tests test_2_LABA.cpp)
target_link_libraries(menu_tests gtest_main)
target_compile_options(menu_tests PRIVATE /W4)

add_test(NAME MenuTests COMMAND menu_tests)

# Основная программа
add_executable(menu_app improved_2_LABA.cpp)
target_compile_options(menu_app PRIVATE /W4)
