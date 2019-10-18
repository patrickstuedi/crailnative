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

#include "crail/client/storage/narpc/narpc_write_response.h"

NarpcWriteResponse::NarpcWriteResponse(RpcClient *client)
    : NarpcStorageResponse(-1, -1), client_(client), ret_(-1) {}

NarpcWriteResponse::~NarpcWriteResponse() {}

int NarpcWriteResponse::Update(ByteBuffer &buffer) {
  NarpcStorageResponse::Update(buffer);

  ret_ = buffer.GetInt();

  return 0;
}

int NarpcWriteResponse::get() {
  client_->PollResponse();
  return ret_;
}
