#include "write_response.h"

WriteResponse::WriteResponse() : StorageResponse(-1, -1), ret_(-1) {}

WriteResponse::~WriteResponse() {}

int WriteResponse::Write(ByteBuffer &buf) const {
  StorageResponse::Write(buf);

  buf.PutInt(ret_);
}

int WriteResponse::Update(ByteBuffer &buf) {
  StorageResponse::Update(buf);

  ret_ = buf.GetInt();
}
