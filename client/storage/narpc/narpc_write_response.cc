#include "narpc_write_response.h"

NarpcWriteResponse::NarpcWriteResponse(RpcChecker *rpc_checker)
    : NarpcStorageResponse(rpc_checker, -1, -1), ret_(-1) {}

NarpcWriteResponse::~NarpcWriteResponse() {}

int NarpcWriteResponse::Write(ByteBuffer &buf) const {
  NarpcStorageResponse::Write(buf);

  buf.PutInt(ret_);

  return 0;
}

int NarpcWriteResponse::Update(ByteBuffer &buf) {
  NarpcStorageResponse::Update(buf);

  ret_ = buf.GetInt();

  return 0;
}
