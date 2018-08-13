#include "namenode_response.h"

NamenodeResponse::NamenodeResponse(RpcChecker *rpc_checker)
    : RpcResponse(rpc_checker), type_(-1), error_(-1) {}

NamenodeResponse::~NamenodeResponse() {}

int NamenodeResponse::Write(ByteBuffer &buf) const {
  buf.PutShort(type_);
  buf.PutShort(error_);

  return Size();
}

int NamenodeResponse::Update(ByteBuffer &buf) {
  this->type_ = buf.GetShort();
  this->error_ = buf.GetShort();

  return Size();
}

// int NamenodeResponse::Get() { return this->rpc_client_->PollResponse(); }
