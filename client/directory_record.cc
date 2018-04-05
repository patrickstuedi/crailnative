#include "directory_record.h"
#include <iostream>
#include <string.h>

using namespace std;

DirectoryRecord::DirectoryRecord(int valid, string &name)
    : valid_(valid), name_(new string(name)) {}

DirectoryRecord::~DirectoryRecord() {}

int DirectoryRecord::Write(ByteBuffer &buf) const {
  int _length = name_->length();
  buf.PutInt(valid_);
  buf.PutInt(_length);
  buf.PutBytes(name_->c_str(), name_->length());
  return Size();
}

int DirectoryRecord::Update(ByteBuffer &buf) {
  this->valid_ = buf.GetInt();
  int _length = buf.GetInt();
  char _tmp[_length];
  buf.GetBytes(_tmp, _length);
  _tmp[_length - 1] = '\0';
  name_.reset(new string(_tmp));
  return Size();
}

int DirectoryRecord::Size() const { return sizeof(int) * 2 + name_->length(); }
