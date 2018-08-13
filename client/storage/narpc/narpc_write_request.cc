#include "narpc_write_request.h"
#include <iostream>

using namespace std;

NarpcWriteRequest::NarpcWriteRequest(int key, long long address, int length,
                                     shared_ptr<ByteBuffer> buf)
    : NarpcStorageRequest(static_cast<int>(NarpcStorageRequestType::Write)),
      key_(key), address_(address), length_(length) {
  this->buf_ = buf;
}

NarpcWriteRequest::~NarpcWriteRequest() {}

int NarpcWriteRequest::Write(ByteBuffer &buf) const {
  NarpcStorageRequest::Write(buf);

  buf.PutInt(key_);
  buf.PutLong(address_);
  buf.PutInt(length_);
  buf.PutInt(length_);

  return 0;
}

int NarpcWriteRequest::Update(ByteBuffer &buf) {
  NarpcStorageRequest::Update(buf);

  key_ = buf.GetInt();
  address_ = buf.GetLong();
  length_ = buf.GetInt();
  length_ = buf.GetInt();

  return 0;
}
