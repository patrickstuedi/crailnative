set(CMAKE_CXX_FLAGS "-fPIC -O3 -g")
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
	common/block_cache.cc
	reflex/reflex_client.cc
	reflex/reflex_header.cc
	reflex/reflex_future.cc
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
	namenode/ioctl_request.cc
	namenode/ioctl_response.cc
	namenode/narpc/narpc_namenode_client.cc
	storage/storage_cache.cc
	storage/narpc/narpc_storage_client.cc
	storage/narpc/narpc_storage_request.cc
	storage/narpc/narpc_storage_response.cc
	storage/narpc/narpc_write_request.cc
	storage/narpc/narpc_write_response.cc
	storage/narpc/narpc_read_request.cc
	storage/narpc/narpc_read_response.cc
	storage/reflex/reflex_storage_client.cc
	metadata/filename.cc
	metadata/file_info.cc
	metadata/datanode_info.cc
	metadata/block_info.cc
	utils/micro_clock.cc
	utils/crail_hash.cc
	utils/crail_networking.cc
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
install (FILES
	common/file_type.h
	common/byte_buffer.h
	common/crail_constants.h
	common/future.h
	common/serializable.h
	common/block_cache.h
	DESTINATION /include/common)
install (FILES
	metadata/block_info.h
	metadata/datanode_info.h
	metadata/file_info.h
	metadata/filename.h 
	DESTINATION /include/metadata)
install (FILES
	namenode/namenode_client.h
	namenode/create_response.h
	namenode/lookup_response.h
	namenode/getblock_response.h
	namenode/remove_response.h
	namenode/void_response.h
	DESTINATION /include/namenode)
