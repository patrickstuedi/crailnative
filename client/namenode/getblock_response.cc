#include "getblock_response.h"

GetblockResponse::GetblockResponse()
    : NamenodeResponse(), block_info_(new BlockInfo()) {}

GetblockResponse::~GetblockResponse() {}

int GetblockResponse::Write(ByteBuffer &buf) const {
  NamenodeResponse::Write(buf);

  block_info_->Write(buf);
  buf.PutShort(error_);

  return 0;
}

int GetblockResponse::Update(ByteBuffer &buf) {
  NamenodeResponse::Update(buf);

  block_info_->Update(buf);
  this->error_ = buf.GetShort();

  return 0;
}
