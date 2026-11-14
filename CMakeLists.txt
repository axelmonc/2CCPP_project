cmake_minimum_required(VERSION 3.16)
project(LieuCulturel CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lieu_culturel_lib STATIC
    src/Room.cpp
    src/Venue.cpp
    src/Event.cpp
    src/Concert.cpp
    src/Play.cpp
    src/Screening.cpp
    src/Workshop.cpp
    src/DataManager.cpp
)
target_include_directories(lieu_culturel_lib PUBLIC include)
target_compile_options(lieu_culturel_lib PUBLIC -Wall -Wextra)

# L'executable : seulement main.cpp
add_executable(lieu_culturel src/main.cpp)
target_link_libraries(lieu_culturel PRIVATE lieu_culturel_lib)

# Tests (desactives par defaut : le build normal ne depend pas de GTest)
option(BUILD_TESTS "Compiler les tests Google Test" OFF)
if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(googletest)

    add_executable(unit_tests
        tests/test_event.cpp
        tests/test_venue.cpp)
    target_link_libraries(unit_tests PRIVATE lieu_culturel_lib GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()