cmake_minimum_required(VERSION 2.8)

project(tuple LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

set(SRC
    main.cpp)

add_executable(${PROJECT_NAME} ${SRC})
