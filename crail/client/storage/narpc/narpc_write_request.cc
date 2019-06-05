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

#include "crail/client/storage/narpc/narpc_write_request.h"
#include <iostream>

using namespace std;

NarpcWriteRequest::NarpcWriteRequest(int key, long long address,
                                     shared_ptr<ByteBuffer> payload)
    : NarpcStorageRequest(static_cast<int>(NarpcStorageRequestType::Write)),
      key_(key), address_(address), length_(payload->remaining()),
      payload_(payload),
      buffer_(sizeof(int) + sizeof(long long) + sizeof(int) * 2) {
  buffer_.PutInt(key_);
  buffer_.PutLong(address_);
  buffer_.PutInt(length_);
  buffer_.PutInt(length_);
  buffer_.Clear();
}

NarpcWriteRequest::~NarpcWriteRequest() {}

int NarpcWriteRequest::Write(NetworkStream &stream) const {
  NarpcStorageRequest::Write(stream);

  cout << "NarpcWriteRequest::Write payload " << payload_->remaining() << endl;
  stream.Write(buffer_.get_bytes(), buffer_.size());
  stream.Write(payload_->get_bytes(), payload_->remaining());
  return 0;
}

int NarpcWriteRequest::Update(NetworkStream &stream) {
  NarpcStorageRequest::Update(stream);

  /*
    key_ = buf.GetInt();
    address_ = buf.GetLong();
    length_ = buf.GetInt();
    length_ = buf.GetInt();
  */

  return 0;
}
