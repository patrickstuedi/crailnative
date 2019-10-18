/*
 * CppCrail: Native Crail
 *
 * Author: Patrick Stuedi  <pstuedi@gmail.com>
 *
 * Copyright (C) 2015-2018, IBM Corporation
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "crail/client/metadata/datanode_info.h"

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

DatanodeInfo::DatanodeInfo() {
  this->ip_address_ = 0;
  this->port_ = 0;
}

DatanodeInfo::~DatanodeInfo() {}

int DatanodeInfo::Write(ByteBuffer &buffer) {
  buffer.PutInt(storage_type_);
  buffer.PutInt(storage_class_);
  buffer.PutInt(location_class_);
  buffer.PutBytes((char *)&ip_address_, 4);
  buffer.PutInt(port_);

  return 0;
}

int DatanodeInfo::Update(ByteBuffer &buffer) {
  storage_type_ = buffer.GetInt();
  storage_class_ = buffer.GetInt();
  location_class_ = buffer.GetInt();
  buffer.GetBytes((char *)&ip_address_, 4);
  port_ = buffer.GetInt();

  return 0;
}

int DatanodeInfo::Dump() const {
  cout << "address " << make_address(ip_address_, port_).c_str();
  return 0;
}

long long DatanodeInfo::Key() const {
  long long key = (((long)ip_address_) << 32) | (port_ & 0xffffffffL);
  return key;
}
