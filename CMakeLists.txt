cmake_minimum_required(VERSION 3.16)
project(LieuCulturel CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lieu_culturel
    main.cpp
    Event.cpp
)
target_compile_options(lieu_culturel PRIVATE -Wall -Wextra)
