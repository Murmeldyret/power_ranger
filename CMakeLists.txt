cmake_minimum_required(VERSION 3.10)

# Project name
project(power_ranger)

# Add json-c library
find_package(json-c REQUIRED)

# Add igraph library
find_package(igraph REQUIRED)

# Add the executable
add_executable(power_ranger src/main.c)
