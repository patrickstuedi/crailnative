#include "datanode_info.h"

#include <iostream>
#include <sstream>

using namespace std;

string make_address(int address, int port) {
  int tmp = address;
  unsigned char *_tmp = (unsigned char *)&tmp;
  stringstream ss;
  for (int i = 0; i < 4; i++) {
    unsigned int ch = (unsigned int)_tmp[i];
    ss << ch;
    if (i < 3) {
      ss << ".";
    }
  }
  ss << "::" << port << endl;

  return ss.str();
}

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

int DatanodeInfo::Dump() const {
  cout << "address " << make_address(ip_address_, port_).c_str();
  return 0;
}
