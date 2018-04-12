set(CMAKE_CXX_FLAGS "-fPIC -std=c++14 -g")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library (cppcrail SHARED 
	crail_store.cc
	crail_node.cc
	crail_file.cc
	crail_directory.cc
	crail_outputstream.cc
	crail_inputstream.cc
	directory_record.cc
	common/byte_buffer.cc
	common/crail_utils.cc
	narpc/rpc_client.cc
	namenode/namenode_request.cc
	namenode/namenode_response.cc
	namenode/create_request.cc
	namenode/create_response.cc
	namenode/lookup_request.cc
	namenode/lookup_response.cc
	namenode/getblock_request.cc
	namenode/getblock_response.cc
	namenode/setfile_request.cc
	namenode/void_response.cc
	namenode/remove_request.cc
	namenode/remove_response.cc
	namenode/namenode_client.cc
	storage/storage_cache.cc
	storage/narpc/narpc_storage_client.cc
	storage/narpc/storage_request.cc
	storage/narpc/storage_response.cc
	storage/narpc/write_request.cc
	storage/narpc/write_response.cc
	storage/narpc/read_request.cc
	storage/narpc/read_response.cc
	metadata/filename.cc
	metadata/file_info.cc
	metadata/datanode_info.cc
	metadata/block_info.cc
	)
#target_link_libraries(cppcrail proto ${PROTOBUF_LIBRARY})

install(TARGETS cppcrail DESTINATION /lib)
install (FILES 
	crail_store.h
	crail_node.h
	crail_file.h
	crail_outputstream.h
	crail_inputstream.h
	directory_record.h
	DESTINATION /include)

