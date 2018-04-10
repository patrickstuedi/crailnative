#include "crail_inputstream.h"

#include <iostream>
#include <memory>

#include "namenode/getblock_response.h"
#include "storage/read_request.h"
#include "storage/read_response.h"
#include "storage/storage_client.h"

CrailInputstream::CrailInputstream(shared_ptr<NamenodeClient> namenode_client,
                                   shared_ptr<FileInfo> file_info,
                                   int position) {
  this->file_info_ = file_info;
  this->namenode_client_ = namenode_client;
  this->position_ = position;
}

CrailInputstream::~CrailInputstream() {}

int CrailInputstream::Read(shared_ptr<ByteBuffer> buf) {
  shared_ptr<GetblockResponse> get_block_res = namenode_client_->GetBlock(
      file_info_->fd(), file_info_->token(), position_, 0);
  shared_ptr<BlockInfo> block_info = get_block_res->block_info();
  int address = block_info->datanode()->addr();
  int port = block_info->datanode()->port();

  StorageClient storage_client{};
  if (storage_client.Connect(address, port) < 0) {
    return -1;
  }
  long long block_addr = block_info->addr() + position_;
  storage_client.ReadData(block_info->lkey(), block_addr, buf);
  return 0;
}
