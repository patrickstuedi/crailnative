#include "namenode_request.h"

NamenodeRequest::NamenodeRequest(short cmd, short type)
    : cmd_(cmd), type_(type) {}

NamenodeRequest::~NamenodeRequest() {}

int NamenodeRequest::Write(ByteBuffer &buf) const {
  buf.PutShort(cmd_);
  buf.PutShort(type_);

  return Size();
}

int NamenodeRequest::Update(ByteBuffer &buf) {
  this->cmd_ = buf.GetShort();
  this->type_ = buf.GetShort();

  return Size();
}
