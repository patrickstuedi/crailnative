#include "datanode_info.h"

#include <iostream>

using namespace std;

DatanodeInfo::DatanodeInfo() {}

DatanodeInfo::~DatanodeInfo() {}

int DatanodeInfo::Write(ByteBuffer &buf) const {
  buf.PutInt(storage_type_);
  buf.PutInt(storage_class_);
  buf.PutInt(location_class_);
  buf.PutBytes((char *)&ip_address_, 4);
  buf.PutInt(port_);

  return 0;
}

int DatanodeInfo::Update(ByteBuffer &buf) {
  storage_type_ = buf.GetInt();
  storage_class_ = buf.GetInt();
  location_class_ = buf.GetInt();
  buf.GetBytes((char *)&ip_address_, 4);
  port_ = buf.GetInt();

  return 0;
}
