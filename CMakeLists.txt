# define project settings
cmake_minimum_required(VERSION 3.14)
project(gtestbox)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(${PROJECT_NAME} STATIC
    person.cpp
)

# hook and configure dependencies
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest EXCLUDE_FROM_ALL)
enable_testing()

# add; link; and catalog test_hello tests
add_executable(test_defaults tests/test_defaults.cpp)
target_include_directories(test_defaults PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(test_defaults gtest gtest_main ${PROJECT_NAME})
add_test(NAME test_defaults COMMAND test_defaults)
