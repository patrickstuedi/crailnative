PROJECT(rpc)
cmake_minimum_required(VERSION 2.6)
add_subdirectory(client)
include_directories ("${PROJECT_SOURCE_DIR}/client")
add_subdirectory(kvdispatcher)
include_directories ("${PROJECT_SOURCE_DIR}/kvdispatcher")
add_subdirectory(iobench)

