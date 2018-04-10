#include "crail_directory.h"

#include <iostream>
#include <memory>

#include "crail_inputstream.h"
#include "directory_record.h"

CrailDirectory::CrailDirectory(shared_ptr<FileInfo> file_info,
                               shared_ptr<NamenodeClient> namenode_client)
    : CrailNode(file_info) {
  this->namenode_client_ = namenode_client;
}

CrailDirectory::~CrailDirectory() {}

int CrailDirectory::Enumerate() {
  unique_ptr<CrailInputstream> input_stream =
      make_unique<CrailInputstream>(namenode_client_, file_info_, 0);
  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(512);
  input_stream->Read(buf);
  buf->Clear();
  DirectoryRecord record;
  record.Update(*buf);
  cout << "record.valid " << record.valid() << ", record.name "
       << record.name().c_str() << endl;

  return 0;
}
