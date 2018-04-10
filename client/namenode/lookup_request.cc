#include "lookup_request.h"

LookupRequest::LookupRequest(Filename &name)
    : NamenodeRequest(static_cast<short>(RpcCommand::Lookup),
                      static_cast<short>(RequestType::Lookup)),
      filename_(name) {
  this->filename_ = std::move(name);
}
LookupRequest::~LookupRequest() {}

int LookupRequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  filename_.Write(buf);
  buf.PutInt(1);

  return Size();
}

int LookupRequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  filename_.Update(buf);
  buf.GetInt();

  return Size();
}
