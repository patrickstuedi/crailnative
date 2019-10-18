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

#ifndef NARPC_READ_RESPONSE_H
#define NARPC_READ_RESPONSE_H

#include <memory>

#include "crail/client//storage/narpc/narpc_storage_response.h"
#include "crail/client/common/future.h"
#include "narpc/rpc_client.h"
#include "narpc/rpc_message.h"

using namespace std;

class NarpcReadResponse : public NarpcStorageResponse, public AsyncTask<int> {
public:
  NarpcReadResponse(RpcClient *client, shared_ptr<ByteBuffer> payload);
  virtual ~NarpcReadResponse();

  virtual shared_ptr<ByteBuffer> Payload() { return payload_; }
  virtual int Update(ByteBuffer &buffer);

  int Size() const { return NarpcStorageResponse::Size() + sizeof(int); }

  int get();
  virtual string ToString() const {
    return "NarpcReadResponse:: " + to_string(length_);
  }

private:
  RpcClient *client_;
  int length_;
  shared_ptr<ByteBuffer> payload_;
};

#endif /* NARPC_READ_RESPONSE_H */
