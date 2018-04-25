#include "crail_file.h"

CrailFile::CrailFile(shared_ptr<FileInfo> file_info,
                     shared_ptr<NamenodeClient> namenode_client,
                     shared_ptr<StorageCache> storage_cache,
                     shared_ptr<BlockCache> block_cache)
    : CrailNode(file_info) {
  this->namenode_client_ = namenode_client;
  this->storage_cache_ = storage_cache;
  this->block_cache_ = block_cache;
}

CrailFile::~CrailFile() {}

unique_ptr<CrailOutputstream> CrailFile::outputstream() {
  return make_unique<CrailOutputstream>(namenode_client_, storage_cache_,
                                        block_cache_, file_info_, 0);
}

unique_ptr<CrailInputstream> CrailFile::inputstream() {
  return make_unique<CrailInputstream>(namenode_client_, storage_cache_,
                                       block_cache_, file_info_, 0);
}
