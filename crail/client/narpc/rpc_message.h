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

#ifndef RPC_MESSAGE_H
#define RPC_MESSAGE_H

#include <memory>

#include "crail/client/common/byte_buffer.h"
#include "crail/client/common/serializable.h"

using namespace std;

class RpcMessage {
public:
  RpcMessage() = default;
  RpcMessage(shared_ptr<Serializable> header,
             shared_ptr<ByteBuffer> payload = nullptr)
      : header_(header), payload_(payload) {}
  RpcMessage(const RpcMessage &message) {
    this->header_ = message.header_;
    this->payload_ = message.payload_;
  }

  RpcMessage &operator=(const RpcMessage &other) {
    if (&other == this) {
      return *this;
    }

    this->header_ = other.header_;
    this->payload_ = other.payload_;
    return *this;
  }

  shared_ptr<Serializable> Header() { return header_; };
  shared_ptr<ByteBuffer> Payload() { return payload_; };

private:
  shared_ptr<Serializable> header_;
  shared_ptr<ByteBuffer> payload_;
};

#endif /* RPC_MESSAGE_H */
