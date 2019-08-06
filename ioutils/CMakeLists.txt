set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -D_GLIBCXX_USE_CXX11_ABI=0")
add_library (ioutils STATIC
	byte_buffer.cc
	)

#install(TARGETS narpc DESTINATION /lib)
#install (FILES 
#	rpc_client.h
#	network_stream.h
#	rpc_message.h
#	DESTINATION /include/narpc)

