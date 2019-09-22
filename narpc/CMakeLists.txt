set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -D_GLIBCXX_USE_CXX11_ABI=0")
add_library (narpc STATIC
	rpc_client.cc
	network_stream.cc
	network_utils.cc
	narpc_staging.cc
	)

target_link_libraries (narpc LINK_PUBLIC ioutils)

#install(TARGETS narpc DESTINATION /lib)
#install (FILES 
#	rpc_client.h
#	network_stream.h
#	rpc_message.h
#	DESTINATION /include/narpc)

