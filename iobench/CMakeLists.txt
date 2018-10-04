set(CMAKE_CXX_FLAGS "-fPIC -std=c++11 -std=c++14 -std=c++17 -O3 -g")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
add_executable(iobench 
	iobench.cc
	)
target_link_libraries (iobench LINK_PUBLIC cppcrail)
install(TARGETS iobench DESTINATION /bin)
