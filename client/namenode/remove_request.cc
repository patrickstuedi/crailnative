#include "remove_request.h"

RemoveRequest::RemoveRequest(Filename &name, bool recursive)
    : NamenodeRequest(static_cast<short>(RpcCommand::Lookup),
                      static_cast<short>(RequestType::Lookup)),
      filename_(name) {
  this->filename_ = std::move(name);
  this->recursive_ = recursive;
}
RemoveRequest::~RemoveRequest() {}

int RemoveRequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  filename_.Write(buf);
  int recursive = this->recursive_ ? 1 : 0;
  buf.PutInt(recursive);

  return Size();
}

int RemoveRequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  filename_.Update(buf);
  buf.GetInt();

  return Size();
}
