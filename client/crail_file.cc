#include "crail_file.h"

CrailFile::CrailFile(shared_ptr<FileInfo> file_info,
                     shared_ptr<NamenodeClient> namenode_client)
    : CrailNode(file_info) {
  this->namenode_client_ = namenode_client;
}

CrailFile::~CrailFile() {}

unique_ptr<CrailOutputstream> CrailFile::outputstream() {
  return make_unique<CrailOutputstream>(namenode_client_, file_info_, 0);
}
