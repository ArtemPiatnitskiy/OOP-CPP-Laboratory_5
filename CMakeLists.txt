cmake_minimum_required(VERSION 3.10)
project(Laboratory_5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# Установка Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

# Отключаем использование статической библиотеки и переключаем на динамическую
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Исходные файлы
set(SOURCES
    src/fixed_block_memory_resource.cpp
)

# Библиотека
add_library(${PROJECT_NAME}_lib ${SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Основной исполняемый файл
add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)

# Добавление тестов
enable_testing()

# Тесты для memory_resource
add_executable(test_memory_resource tests/test_memory_resource.cpp)
target_link_libraries(test_memory_resource PRIVATE ${PROJECT_NAME}_lib gtest_main)
add_test(NAME Laboratory_5_tests_memory_resource COMMAND test_memory_resource)

# Тесты для doubly_linked_list
add_executable(test_doubly_linked_list tests/test_doubly_linked_list.cpp)
target_link_libraries(test_doubly_linked_list PRIVATE ${PROJECT_NAME}_lib gtest_main)
add_test(NAME Laboratory_5_tests_doubly_linked_list COMMAND test_doubly_linked_list)

# Тесты для структур
add_executable(test_struct tests/test_struct.cpp)
target_link_libraries(test_struct PRIVATE ${PROJECT_NAME}_lib gtest_main)
add_test(NAME Laboratory_5_tests_struct COMMAND test_struct)

# Тесты для итератора
add_executable(test_iterator_${PROJECT_NAME} tests/test_iterator.cpp)
target_link_libraries(test_iterator_${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib gtest_main)
add_test(NAME Laboratory_5_tests_iterator COMMAND test_iterator_${PROJECT_NAME})