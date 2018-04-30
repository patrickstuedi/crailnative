#include "ioctl_response.h"

IoctlResponse::IoctlResponse() : NamenodeResponse() {}

IoctlResponse::~IoctlResponse() {}

int IoctlResponse::Write(ByteBuffer &buf) const {
  NamenodeResponse::Write(buf);

  buf.PutByte(op_);
  buf.PutLong(count_);

  return 0;
}

int IoctlResponse::Update(ByteBuffer &buf) {
  NamenodeResponse::Update(buf);

  op_ = buf.GetByte();
  count_ = buf.GetLong();

  return 0;
}
