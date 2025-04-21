cmake_minimum_required(VERSION 3.28.3)

include(CTest)
enable_testing()

# Флаги компиляции
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Флаги для покрытия кода (активны только в Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage -fsanitize=address -fsanitize=leak)
    add_link_options(--coverage -fprofile-arcs -ftest-coverage -fsanitize=address -fsanitize=leak)
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
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_library(my_deque ${SRC_FILES})
target_include_directories(my_deque PUBLIC include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(my_deque PRIVATE asan)
endif()

# Создаём отдельный исполняемый файл для тестов
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS test/*.cpp)
add_executable(tests_deque ${TEST_FILES})
target_link_libraries(tests_deque PRIVATE my_deque GTest::gtest_main)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(tests_deque PRIVATE asan)
endif()

# Регистрируем тесты
add_test(NAME MyTests COMMAND tests_deque)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Добавляем цель для покрытия кода
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if(LCOV AND GENHTML)
        add_custom_target(coverage_deque
                COMMAND ${LCOV} --capture --directory test --ignore-errors mismatch --output-file coverage.info
                COMMAND ${LCOV} --remove coverage.info /usr/* */googletest/* */tests/* --output-file coverage_filtered.info
                COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Генерация отчёта покрытия кода..."
                VERBATIM
        )
    else()
        message(WARNING "lcov или genhtml не найдены, цель 'coverage' недоступна.")
    endif()
endif()

# Добавляем цель для анализа кода cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    add_custom_target(cppcheck_deque
            COMMAND ${CPPCHECK} --enable=all --inconclusive --quiet --suppress=missingIncludeSystem -I include src test
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()

add_custom_target(clean-build_deque
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*.a
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/test*
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/coverage_filtered.info
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/coverage.info
        COMMENT "Очистка собранных исполняемых файлов"
)

add_custom_target(purge_deque
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
        COMMENT "Полная очистка всех артефактов сборки"
)

add_executable(main_deque src/main.cpp)
target_link_libraries(main_deque PRIVATE my_deque)