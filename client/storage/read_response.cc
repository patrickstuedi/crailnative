#include "read_response.h"

#include <iostream>

using namespace std;

ReadResponse::ReadResponse(shared_ptr<ByteBuffer> payload)
    : StorageResponse(-1, -1), length_(-1) {
  this->payload_ = payload;
}

ReadResponse::~ReadResponse() {}

int ReadResponse::Write(ByteBuffer &buf) const {
  StorageResponse::Write(buf);

  buf.PutInt(length_);

  return 0;
}

int ReadResponse::Update(ByteBuffer &buf) {
  StorageResponse::Update(buf);

  length_ = buf.GetInt();

  return 0;
}
