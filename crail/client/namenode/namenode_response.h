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

#ifndef NAMENODE_RESPONSE_H
#define NAMENODE_RESPONSE_H

#include <ostream>

#include "crail/client/common/byte_buffer.h"
#include "crail/client/common/serializable.h"
#include "narpc/network_stream.h"
#include "narpc/rpc_message.h"

using namespace std;

class NamenodeResponse : public RpcMessage {
public:
  NamenodeResponse();
  virtual ~NamenodeResponse();

  int Write(NetworkStream &stream) const;
  int Update(NetworkStream &stream);
  int Size() const { return sizeof(short) * 2; }

  int type() {
    Sync();
    return type_;
  }

  int error() {
    Sync();
    return error_;
  }

  string ToString() const { return "NamenodeResponse"; }

private:
  void Sync();

  short type_;
  short error_;
  ByteBuffer buffer_;
};

#endif /* NAMENODE_RESPONSE_H */
