set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -D_GLIBCXX_USE_CXX11_ABI=0")
add_library (narpc SHARED 
	rpc_client.cc
	network_stream.cc
	network_utils.cc
	rpc_message.h
	)

install(TARGETS narpc DESTINATION /lib)
install (FILES 
	rpc_client.h
	network_stream.h
	rpc_message.h
	DESTINATION /include/narpc)

