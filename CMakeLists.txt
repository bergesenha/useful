cmake_minimum_required(VERSION 3.13)


project(useful CXX)

set(CMAKE_CXX_EXTENSIONS OFF)


option(USEFUL_BUILD_TESTS "Build unit tests." OFF)

add_library(useful INTERFACE)
target_compile_features(useful INTERFACE cxx_std_17)
target_include_directories(useful
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

if(USEFUL_BUILD_TESTS)
    find_package(Catch2)

    add_executable(unit_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp)
    target_link_libraries(unit_tests
        PRIVATE Catch2::Catch2
        PRIVATE useful
        )
    target_sources(unit_tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_ecs.cpp
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_point_traits.cpp
        )
endif(USEFUL_BUILD_TESTS)
