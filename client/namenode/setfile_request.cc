#include "setfile_request.h"

SetfileRequest::SetfileRequest(shared_ptr<FileInfo> file_info, bool close)
    : NamenodeRequest(static_cast<short>(RpcCommand::Setfile),
                      static_cast<short>(RequestType::Setfile)) {
  this->file_info_ = file_info;
  this->close_ = close;
}

SetfileRequest::~SetfileRequest() {}

int SetfileRequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  file_info_->Write(buf);
  int _close = close_ ? 1 : 0;
  buf.PutInt(_close);

  return Size();
}

int SetfileRequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  file_info_->Update(buf);
  buf.GetInt();

  return Size();
}
