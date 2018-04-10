#include "lookup_response.h"

#include <iostream>

using namespace std;

LookupResponse::LookupResponse()
    : NamenodeResponse(), file_info_(new FileInfo()),
      block_info_(new BlockInfo()) {}

LookupResponse::~LookupResponse() {}

int LookupResponse::Write(ByteBuffer &buf) const {
  NamenodeResponse::Write(buf);

  file_info_->Write(buf);
  block_info_->Write(buf);

  return 0;
}

int LookupResponse::Update(ByteBuffer &buf) {
  NamenodeResponse::Update(buf);

  file_info_->Update(buf);
  block_info_->Update(buf);

  return 0;
}
