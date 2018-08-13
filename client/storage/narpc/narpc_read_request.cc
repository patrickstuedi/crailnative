#include "narpc_read_request.h"

NarpcReadRequest::NarpcReadRequest(int key, long long address, int length)
    : NarpcStorageRequest(static_cast<int>(NarpcStorageRequestType::Read)),
      key_(key), address_(address), length_(length) {}

NarpcReadRequest::~NarpcReadRequest() {}

int NarpcReadRequest::Write(ByteBuffer &buf) const {
  NarpcStorageRequest::Write(buf);

  buf.PutInt(key_);
  buf.PutLong(address_);
  buf.PutInt(length_);
  // buf.PutInt(length_);

  return 0;
}

int NarpcReadRequest::Update(ByteBuffer &buf) {
  NarpcStorageRequest::Update(buf);

  key_ = buf.GetInt();
  address_ = buf.GetLong();
  length_ = buf.GetInt();
  // length_ = buf.GetInt();

  return 0;
}
