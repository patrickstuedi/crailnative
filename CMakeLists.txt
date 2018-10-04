PROJECT(rpc)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(client)
include_directories ("${PROJECT_SOURCE_DIR}/client")
add_subdirectory(pocket)
include_directories ("${PROJECT_SOURCE_DIR}/pocket")
add_subdirectory(shell)
add_subdirectory(iobench)

