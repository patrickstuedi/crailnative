/*
 * CppCrail: Native Crail
 *
 * Author: Patrick Stuedi  <pstuedi@gmail.com>
 *
 * Copyright (C) 2015-2018
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
#include <ostream>
#include <string>

#include "ioutils/byte_buffer.h"

using namespace std;
using namespace crail;

class RpcMessage {
public:
  virtual int Update(ByteBuffer &buffer) = 0;
  virtual int Write(ByteBuffer &buffer) = 0;
  virtual shared_ptr<ByteBuffer> Payload() = 0;

  virtual int Size() const = 0;
  virtual string ToString() const = 0;

  friend ostream &operator<<(ostream &stream, RpcMessage const &message) {
    stream << message.ToString();
    return stream;
  }

  friend ostream &operator<<(ostream &stream, RpcMessage *message) {
    stream << message->ToString();
    return stream;
  }

  friend ostream &operator<<(ostream &stream, unique_ptr<RpcMessage> message) {
    stream << message->ToString();
    return stream;
  }

  friend ostream &operator<<(ostream &stream, shared_ptr<RpcMessage> message) {
    stream << message->ToString();
    return stream;
  }
};

#endif /* RPC_MESSAGE_H */
