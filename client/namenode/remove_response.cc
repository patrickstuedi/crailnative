#include "remove_response.h"

RemoveResponse::RemoveResponse(RpcClient *rpc_client)
    : NamenodeResponse(rpc_client), file_info_(new FileInfo()),
      parent_info_(new FileInfo()) {}

RemoveResponse::~RemoveResponse() {}

int RemoveResponse::Write(ByteBuffer &buf) const {
  NamenodeResponse::Write(buf);

  file_info_->Write(buf);
  parent_info_->Write(buf);

  return 0;
}

int RemoveResponse::Update(ByteBuffer &buf) {
  NamenodeResponse::Update(buf);

  file_info_->Update(buf);
  parent_info_->Update(buf);

  return 0;
}
