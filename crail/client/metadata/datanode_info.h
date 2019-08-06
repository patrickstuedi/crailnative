/*
 * CppCrail: Native Crail
 *
 * Author: Patrick Stuedi  <stu@zurich.ibm.com>
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

#ifndef DATANODE_INFO_H
#define DATANODE_INFO_H

#include "crail/client/common/serializable.h"
#include "crail/client/utils/crail_networking.h"
#include "ioutils/byte_buffer.h"

using namespace crail;

class DatanodeInfo : public Serializable {
public:
  DatanodeInfo();
  DatanodeInfo(const DatanodeInfo &dn_info) = default;
  virtual ~DatanodeInfo();

  int Write(ByteBuffer &buf) const;
  int Update(ByteBuffer &buf);

  int Size() const {
    return sizeof(int) * 3 + sizeof(unsigned char) * 4 + sizeof(int);
  }

  int Dump() const;
  long long Key() const;

  int storage_type() const { return storage_type_; }
  int storage_class() const { return storage_class_; }
  int location_class() const { return location_class_; }
  int port() const { return port_; }
  int addr() { return ip_address_; }
  bool valid() { return ip_address_ && port_; }
  string ToString() const {
    return "DatanodeInfo:: " + GetAddress(ip_address_, port_);
  }
  DatanodeInfo &operator=(const DatanodeInfo &other) {
    storage_type_ = other.storage_type_;
    storage_class_ = other.storage_class_;
    location_class_ = other.location_class_;
    ip_address_ = other.ip_address_;
    port_ = other.port_;

    return *this;
  }

private:
  int storage_type_;
  int storage_class_;
  int location_class_;
  int ip_address_;
  int port_;
};

#endif /* DATANODE_INFO_H */
