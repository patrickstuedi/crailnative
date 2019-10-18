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

#include "crail/client/storage/narpc/narpc_read_response.h"

#include <iostream>

using namespace std;

NarpcReadResponse::NarpcReadResponse(RpcClient *client,
                                     shared_ptr<ByteBuffer> buf)
    : NarpcStorageResponse(-1, -1), client_(client), length_(-1),
      payload_(buf) {}

NarpcReadResponse::~NarpcReadResponse() {}

int NarpcReadResponse::Update(ByteBuffer &buffer) {
  NarpcStorageResponse::Update(buffer);

  length_ = buffer.GetInt();

  payload_->Clear();
  payload_->set_limit(length_);

  return 0;
}

int NarpcReadResponse::get() {
  client_->PollResponse();
  return length_;
}
