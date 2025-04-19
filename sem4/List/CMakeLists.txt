cmake_minimum_required(VERSION 3.28.3)
enable_testing()

# Флаги компиляции
set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak)

# Флаги для покрытия кода (активны только в Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS
            --coverage
            -fprofile-arcs
            -ftest-coverage
    )
endif()

# Добавляем GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем библиотеку
add_library(my_list src/my_list.cpp
        src/main.cpp)
target_include_directories(my_list PUBLIC include)
target_compile_options(my_list PRIVATE ${COMMON_FLAGS} ${COVERAGE_FLAGS})
target_link_libraries(my_list PRIVATE ${COVERAGE_FLAGS})

# Создаём отдельный исполняемый файл для тестов
add_executable(tests_list test/test_my_list.cpp)
target_link_libraries(tests_list PRIVATE my_list GTest::gtest_main)
target_compile_options(tests_list PRIVATE ${COMMON_FLAGS} ${COVERAGE_FLAGS})
target_link_libraries(tests_list PRIVATE ${COVERAGE_FLAGS})

# Регистрируем тесты
add_test(NAME MyTests COMMAND tests_list)

# Добавляем цель для покрытия кода
find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(LCOV AND GENHTML)
    add_custom_target(coverage_list
            COMMAND ${LCOV} --capture --directory .. --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/googletest/*' --output-file coverage_filtered.info
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Генерация отчёта покрытия кода..."
            VERBATIM
    )
else()
    message(WARNING "lcov или genhtml не найдены, цель 'coverage' недоступна.")
endif()

# Добавляем цель для анализа кода cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    add_custom_target(cppcheck_list
            COMMAND ${CPPCHECK} --enable=all --inconclusive --quiet --suppress=missingIncludeSystem -I include src test
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()

add_executable(main_list src/main.cpp)
target_link_libraries(main_list PRIVATE my_list)