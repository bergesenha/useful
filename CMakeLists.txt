cmake_minimum_required(VERSION 3.13)


project(useful CXX)

set(CMAKE_CXX_EXTENSIONS OFF)


option(USEFUL_BUILD_TESTS "Build unit tests." OFF)


if(USEFUL_BUILD_TESTS)
    find_package(Catch2)

    add_executable(unit_tests tests/test_main.cpp)
    target_link_libraries(unit_tests
        PRIVATE Catch2::Catch2
        )
endif(USEFUL_BUILD_TESTS)
