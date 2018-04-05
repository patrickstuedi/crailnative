#include "block_info.h"

BlockInfo::BlockInfo() { this->datanode_info_ = new DatanodeInfo(); }

BlockInfo::~BlockInfo() { delete datanode_info_; }

int BlockInfo::Write(ByteBuffer &buf) const {
  datanode_info_->Write(buf);
  buf.PutLong(lba_);
  buf.PutLong(addr_);
  buf.PutInt(length_);
  buf.PutInt(lkey_);

  return 0;
}

int BlockInfo::Update(ByteBuffer &buf) {
  datanode_info_->Update(buf);
  lba_ = buf.GetLong();
  addr_ = buf.GetLong();
  length_ = buf.GetInt();
  lkey_ = buf.GetInt();

  return 0;
}
