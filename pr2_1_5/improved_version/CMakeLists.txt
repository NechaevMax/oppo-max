cmake_minimum_required(VERSION 3.16)
project(menu_app_improved LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(menu_lib
    src/MenuLineParser.cpp
    src/MenuRepository.cpp
    src/MenuManager.cpp
)

target_include_directories(menu_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(menu_lib PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

add_executable(menu_app src/main.cpp)
target_link_libraries(menu_app PRIVATE menu_lib)

add_executable(menu_tests tests/test_main.cpp)
target_link_libraries(menu_tests PRIVATE menu_lib)

enable_testing()
add_test(NAME MenuTests COMMAND menu_tests)

add_executable(menu_benchmark src/benchmark.cpp)
target_link_libraries(menu_benchmark PRIVATE menu_lib)
