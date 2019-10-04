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

#ifndef BLOCK_INFO_H
#define BLOCK_INFO_H

#include <string>

#include "crail/client/common/serializable.h"
#include "crail/client/metadata/datanode_info.h"
#include "ioutils/byte_buffer.h"
#include "narpc/network_stream.h"

using namespace std;

class BlockInfo {
public:
  BlockInfo();
  // BlockInfo(const BlockInfo &block_info) = default;
  virtual ~BlockInfo();

  virtual int UpdateMetadata(ByteBuffer &buffer);
  virtual int WriteMetadata(ByteBuffer &buffer);

  int Write(NetworkStream &stream) const;
  int Update(NetworkStream &stream);

  int Size() const {
    return datanode_info_.Size() + sizeof(unsigned long long) * 2 +
           sizeof(int) * 2;
  }

  int Dump() const;

  DatanodeInfo datanode() const { return datanode_info_; }
  unsigned long long lba() const { return lba_; }
  unsigned long long addr() const { return addr_; }
  int length() const { return length_; }
  int lkey() const { return lkey_; }
  bool valid() { return datanode_info_.valid(); }
  string ToString() const {
    return "BlockInfo:: " + to_string(lba_) + ", " + to_string(addr_) + ", " +
           to_string(length_) + " " + datanode_info_.ToString();
  }

  /*
BlockInfo &operator=(const BlockInfo &other) {
datanode_info_ = other.datanode_info_;
lba_ = other.lba_;
addr_ = other.addr_;
length_ = other.length_;
lkey_ = other.lkey_;
return *this;
}
  */

private:
  DatanodeInfo datanode_info_;
  unsigned long long lba_;
  unsigned long long addr_;
  int length_;
  int lkey_;
};

#endif /* BLOCK_INFO_H */
