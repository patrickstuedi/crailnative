#include "ioctl_request.h"

IoctlRequest::IoctlRequest(unsigned char op, Filename &name)
    : NamenodeRequest(static_cast<short>(RpcCommand::Ioctl),
                      static_cast<short>(RequestType::Ioctl)),
      filename_(name) {
  this->op_ = op;
  this->filename_ = std::move(name);
}

IoctlRequest::~IoctlRequest() {}

int IoctlRequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  buf.PutByte(op_);
  filename_.Write(buf);

  return Size();
}

int IoctlRequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  op_ = buf.GetByte();
  filename_.Update(buf);

  return Size();
}
