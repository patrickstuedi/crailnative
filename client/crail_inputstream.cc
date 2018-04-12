#include "crail_inputstream.h"

#include <iostream>
#include <memory>

#include "namenode/getblock_response.h"
#include "storage/narpc/narpc_storage_client.h"
#include "storage/storage_client.h"

CrailInputstream::CrailInputstream(shared_ptr<NamenodeClient> namenode_client,
                                   shared_ptr<StorageCache> storage_cache,
                                   shared_ptr<FileInfo> file_info,
                                   int position) {
  this->file_info_ = file_info;
  this->namenode_client_ = namenode_client;
  this->storage_cache_ = storage_cache;
  this->position_ = position;
}

CrailInputstream::~CrailInputstream() {}

int CrailInputstream::Read(shared_ptr<ByteBuffer> buf) {
  if (position_ >= file_info_->capacity()) {
    return -1;
  }

  int buf_original_limit = buf->limit();
  int block_offset = position_ % kBlockSize;
  int block_remaining = kBlockSize - block_offset;
  int file_remaining = file_info_->capacity() - position_;

  if (block_remaining < buf->remaining()) {
    buf->set_limit(buf->position() + block_remaining);
  }
  if (file_remaining < buf->remaining()) {
    buf->set_limit(buf->position() + file_remaining);
  }

  shared_ptr<GetblockResponse> get_block_res = namenode_client_->GetBlock(
      file_info_->fd(), file_info_->token(), position_, 0);
  shared_ptr<BlockInfo> block_info = get_block_res->block_info();
  int address = block_info->datanode()->addr();
  int port = block_info->datanode()->port();

  shared_ptr<StorageClient> storage_client =
      storage_cache_->Get(block_info->datanode()->Key());
  if (storage_client->Connect(address, port) < 0) {
    return -1;
  }

  long long block_addr = block_info->addr() + block_offset;
  storage_client->ReadData(block_info->lkey(), block_addr, buf);
  storage_cache_->Put(block_info->datanode()->Key(), storage_client);

  int len = buf->remaining();
  this->position_ += buf->remaining();
  buf->set_position(buf->position() + buf->remaining());
  buf->set_limit(buf_original_limit);

  return len;
}

int CrailInputstream::Close() {}
