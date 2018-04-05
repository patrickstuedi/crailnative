set(CMAKE_CXX_FLAGS "-fPIC -std=c++14 -g")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_library (kvdispatcher SHARED 
	crail_dispatcher.cc
	)

target_link_libraries (kvdispatcher LINK_PUBLIC cppcrail)
install(TARGETS kvdispatcher DESTINATION /lib)
install (FILES 
	crail_dispatcher.h
	DESTINATION /include)

