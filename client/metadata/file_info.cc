#include "file_info.h"

#include <iostream>

using namespace std;

FileInfo::FileInfo() {}

FileInfo::~FileInfo() {}

int FileInfo::Write(ByteBuffer &buf) const {
  buf.PutLong(fd_);
  buf.PutLong(capacity_);
  buf.PutInt(node_type_);
  buf.PutLong(dir_offset_);
  buf.PutLong(token_);
  buf.PutLong(modification_time_);

  return 0;
}

int FileInfo::Update(ByteBuffer &buf) {
  fd_ = buf.GetLong();
  capacity_ = buf.GetLong();
  node_type_ = buf.GetInt();
  dir_offset_ = buf.GetLong();
  token_ = buf.GetLong();
  modification_time_ = buf.GetLong();

  return 0;
}

int FileInfo::Dump() const {
  cout << "fd " << fd_ << ", capacity " << capacity_ << ", type " << node_type_
       << endl;
  return 0;
}
