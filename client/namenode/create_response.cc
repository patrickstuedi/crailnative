#include "create_response.h"

CreateResponse::CreateResponse()
    : NamenodeResponse(), file_info_(new FileInfo()),
      parent_info_(new FileInfo()), file_block_(new BlockInfo()),
      parent_block_(new BlockInfo()) {}

CreateResponse::~CreateResponse() {}

int CreateResponse::Write(ByteBuffer &buf) const {
  NamenodeResponse::Write(buf);

  file_info_->Write(buf);
  parent_info_->Write(buf);
  file_block_->Write(buf);
  parent_block_->Write(buf);

  return 0;
}

int CreateResponse::Update(ByteBuffer &buf) {
  NamenodeResponse::Update(buf);

  file_info_->Update(buf);
  parent_info_->Update(buf);
  file_block_->Update(buf);
  parent_block_->Update(buf);

  return 0;
}
