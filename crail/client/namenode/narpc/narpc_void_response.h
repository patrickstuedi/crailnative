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

#ifndef NARPC_VOID_RESPONSE_H
#define NARPC_VOID_RESPONSE_H

#include "crail/client/common/future.h"
#include "crail/client/namenode/void_response.h"
#include "narpc/rpc_client.h"

class NarpcVoidResponse : public VoidResponse, public AsyncTask<VoidResponse> {
public:
  NarpcVoidResponse(RpcClient *client) : VoidResponse(), client_(client) {}
  virtual ~NarpcVoidResponse() {}

  VoidResponse get() {
    client_->PollResponse();
    return *this;
  }

private:
  RpcClient *client_;
};

#endif /* NARPC_VOID_RESPONSE_H */
