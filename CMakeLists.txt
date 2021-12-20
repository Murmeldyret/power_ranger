cmake_minimum_required(VERSION 3.16)

# Project name
project(power_ranger)
project(test_ranger)

# Add igraph library
find_package(igraph REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

# Add the executable
add_executable(power_ranger src/main.c src/algorithms.c src/data.c src/output.c src/simulation.c)
add_executable(test_ranger tests/test.c)

target_link_libraries(power_ranger PUBLIC igraph::igraph)

# target include directory for config folder
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config)

add_custom_command(TARGET power_ranger POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config/ $<TARGET_FILE_DIR:power_ranger>/config)