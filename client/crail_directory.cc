#include "crail_directory.h"

#include <iostream>
#include <memory>

#include "crail_inputstream.h"
#include "directory_record.h"

CrailDirectory::CrailDirectory(shared_ptr<FileInfo> file_info,
                               shared_ptr<NamenodeClient> namenode_client,
                               shared_ptr<StorageCache> storage_cache,
                               shared_ptr<BlockCache> block_cache)
    : CrailNode(file_info) {
  this->namenode_client_ = namenode_client;
  this->storage_cache_ = storage_cache;
  this->block_cache_ = block_cache;
}

CrailDirectory::~CrailDirectory() {}

int CrailDirectory::Enumerate() {
  int records = file_info_->capacity() / 512;
  unique_ptr<CrailInputstream> input_stream = make_unique<CrailInputstream>(
      namenode_client_, storage_cache_, block_cache_, file_info_, 0);
  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(512);
  DirectoryRecord record;
  for (int i = 0; i < records; i++) {
    buf->Clear();
    input_stream->Read(buf);
    buf->Flip();
    record.Update(*buf);
    if (record.valid()) {
      cout << record.name().c_str() << endl;
    }
  }

  return 0;
}
