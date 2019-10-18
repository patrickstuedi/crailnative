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

#ifndef NARPC_WRITE_REQUEST_H
#define NARPC_WRITE_REQUEST_H

#include <memory>
#include <vector>

#include "crail/client/common/serializable.h"
#include "crail/client/storage/narpc/narpc_storage_request.h"
#include "ioutils/byte_buffer.h"
#include "narpc/rpc_client.h"

using namespace std;

class NarpcWriteRequest : public NarpcStorageRequest {
public:
  NarpcWriteRequest(int key, long long address, shared_ptr<ByteBuffer> payload);
  virtual ~NarpcWriteRequest();

  virtual int Write(ByteBuffer &buffer);
  virtual shared_ptr<ByteBuffer> Payload() { return payload_; }

  int Size() const {
    return NarpcStorageRequest::Size() + sizeof(int) + sizeof(long long) +
           sizeof(int) * 2 + payload_->remaining();
  }

  string ToString() const {
    return "NarpcWriteRequest:: " + to_string(key_) + ", " +
           to_string(address_) + ", " + to_string(length_) + ", " +
           to_string(payload_->remaining());
  }

private:
  int key_;
  long long address_;
  int length_;
  shared_ptr<ByteBuffer> payload_;
};

#endif /* NARPC_WRITE_REQUEST_H */
