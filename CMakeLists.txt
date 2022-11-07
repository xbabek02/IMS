cmake_minimum_required(VERSION 3.19)

project(ims)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set(IMS_SOURCES
    bomber.hpp
    bomber.cpp
    fighter.cpp
    fighter.hpp
    plane.cpp
    plane.hpp
    withid.hpp
    withid.cpp
    grid.hpp
    grid.cpp
    simulation.cpp
    simulation.hpp
    )

add_subdirectory(json)

add_executable(${PROJECT_NAME} main.cpp ${IMS_SOURCES})
target_link_libraries(example PRIVATE nlohmann_json::nlohmann_json)