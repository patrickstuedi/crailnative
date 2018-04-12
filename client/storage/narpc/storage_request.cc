#include "storage_request.h"

StorageRequest::StorageRequest(int type) : type_(type) {}

StorageRequest::~StorageRequest() {}

int StorageRequest::Write(ByteBuffer &buf) const {
  buf.PutInt(type_);

  return Size();
}

int StorageRequest::Update(ByteBuffer &buf) {
  this->type_ = buf.GetInt();

  return Size();
}
