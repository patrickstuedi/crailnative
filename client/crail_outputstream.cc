#include "crail_outputstream.h"

#include <iostream>
#include <memory>

#include "namenode/getblock_response.h"
#include "storage/storage_client.h"
#include "storage/write_request.h"
#include "storage/write_response.h"

using namespace std;

CrailOutputstream::CrailOutputstream(shared_ptr<NamenodeClient> namenode_client,
                                     shared_ptr<FileInfo> file_info,
                                     int position) {
  this->file_info_ = file_info;
  this->namenode_client_ = namenode_client;
  this->position_ = position;
}

CrailOutputstream::~CrailOutputstream() {}

int CrailOutputstream::Write(shared_ptr<ByteBuffer> buf) {
  shared_ptr<GetblockResponse> get_block_res = namenode_client_->GetBlock(
      file_info_->fd(), file_info_->token(), position_, 0);
  cout << "fd " << file_info_->fd() << ", blockinfo "
       << get_block_res->block_info()->length() << ", address "
       << get_block_res->block_info()->addr() << ", buf rem "
       << buf->remaining() << endl;

  shared_ptr<BlockInfo> block_info = get_block_res->block_info();
  int address = block_info->datanode()->addr();
  int port = block_info->datanode()->port();

  StorageClient storage_client{};
  if (storage_client.Connect(address, port) < 0) {
    return -1;
  }
  storage_client.WriteData(block_info->lkey(), block_info->addr(), buf);
  return 0;
}

int CrailOutputstream::Close() {}
