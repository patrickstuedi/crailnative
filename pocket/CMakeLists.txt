set(CMAKE_CXX_FLAGS "-fPIC -O3 -g")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_library (pocket SHARED 
	pocket_dispatcher.cc
	)

target_link_libraries (pocket LINK_PUBLIC cppcrail)
install(TARGETS pocket DESTINATION /lib)
install (FILES 
	pocket_dispatcher.h
	DESTINATION /include)

