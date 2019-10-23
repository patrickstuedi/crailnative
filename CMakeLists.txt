project(crailnative)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories ("${PROJECT_SOURCE_DIR}")
add_subdirectory(ioutils)
add_subdirectory(narpc)
add_subdirectory(disni)
add_subdirectory(crail)
add_subdirectory(iobench)

