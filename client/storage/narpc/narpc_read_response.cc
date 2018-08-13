#include "narpc_read_response.h"

#include <iostream>

using namespace std;

NarpcReadResponse::NarpcReadResponse(RpcChecker *rpc_checker,
                                     shared_ptr<ByteBuffer> payload)
    : NarpcStorageResponse(rpc_checker, -1, -1), length_(-1) {
  this->payload_ = payload;
}

NarpcReadResponse::~NarpcReadResponse() {}

int NarpcReadResponse::Write(ByteBuffer &buf) const {
  NarpcStorageResponse::Write(buf);

  buf.PutInt(length_);

  return 0;
}

int NarpcReadResponse::Update(ByteBuffer &buf) {
  NarpcStorageResponse::Update(buf);

  length_ = buf.GetInt();

  return 0;
}
