#include "write_request.h"
#include <iostream>

using namespace std;

WriteRequest::WriteRequest(int key, long long address, int length,
                           shared_ptr<ByteBuffer> buf)
    : StorageRequest(static_cast<int>(StorageRequestType::Write)), key_(key),
      address_(address), length_(length) {
  this->buf_ = buf;
}

WriteRequest::~WriteRequest() {}

int WriteRequest::Write(ByteBuffer &buf) const {
  StorageRequest::Write(buf);

  buf.PutInt(key_);
  buf.PutLong(address_);
  buf.PutInt(length_);
  buf.PutInt(length_);

  return 0;
}

int WriteRequest::Update(ByteBuffer &buf) {
  StorageRequest::Update(buf);

  key_ = buf.GetInt();
  address_ = buf.GetLong();
  length_ = buf.GetInt();
  length_ = buf.GetInt();

  return 0;
}
