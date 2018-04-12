#include "read_request.h"

ReadRequest::ReadRequest(int key, long long address, int length)
    : StorageRequest(static_cast<int>(StorageRequestType::Read)), key_(key),
      address_(address), length_(length) {}

ReadRequest::~ReadRequest() {}

int ReadRequest::Write(ByteBuffer &buf) const {
  StorageRequest::Write(buf);

  buf.PutInt(key_);
  buf.PutLong(address_);
  buf.PutInt(length_);
  buf.PutInt(length_);

  return 0;
}

int ReadRequest::Update(ByteBuffer &buf) {
  StorageRequest::Update(buf);

  key_ = buf.GetInt();
  address_ = buf.GetLong();
  length_ = buf.GetInt();
  length_ = buf.GetInt();

  return 0;
}
