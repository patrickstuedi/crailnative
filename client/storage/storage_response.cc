#include "storage_response.h"

#include <iostream>

using namespace std;

StorageResponse::StorageResponse(int error, int type)
    : error_(error), type_(type) {}

StorageResponse::~StorageResponse() {}

int StorageResponse::Write(ByteBuffer &buf) const {
  buf.PutInt(error_);
  buf.PutInt(type_);
  return 0;
}

int StorageResponse::Update(ByteBuffer &buf) {
  error_ = buf.GetInt();
  type_ = buf.GetInt();
  return 0;
}
