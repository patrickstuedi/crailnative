set(CMAKE_CXX_FLAGS "-fPIC -O3 -g")
add_executable(iobench 
	iobench.cc
	)
target_link_libraries (iobench LINK_PUBLIC crail)
install(TARGETS iobench DESTINATION /bin)
