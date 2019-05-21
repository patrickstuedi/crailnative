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

#include "crail/client/storage/narpc/narpc_storage_client.h"

#include <iostream>

#include "crail/client/storage/narpc/narpc_read_request.h"
#include "crail/client/storage/narpc/narpc_read_response.h"
#include "crail/client/storage/narpc/narpc_storage_request.h"
#include "crail/client/storage/narpc/narpc_storage_response.h"
#include "crail/client/storage/narpc/narpc_write_request.h"
#include "crail/client/storage/narpc/narpc_write_response.h"

using namespace std;

NarpcStorageClient::NarpcStorageClient(int address, int port)
    : rpc_client_(address, port, NarpcStorageClient::kNodelay) {}

NarpcStorageClient::~NarpcStorageClient() {}

Future<int> NarpcStorageClient::WriteData(int key, long long address,
                                          shared_ptr<ByteBuffer> buf) {
  shared_ptr<NarpcWriteRequest> request =
      make_shared<NarpcWriteRequest>(key, address, buf->remaining());
  shared_ptr<NarpcWriteResponse> response =
      make_shared<NarpcWriteResponse>(this);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<int>::Failure(-1);
  }
  return Future<int>(response);
}

Future<int> NarpcStorageClient::ReadData(int key, long long address,
                                         shared_ptr<ByteBuffer> buf) {
  shared_ptr<NarpcReadRequest> request =
      make_shared<NarpcReadRequest>(key, address, buf->remaining());
  shared_ptr<NarpcReadResponse> response = make_shared<NarpcReadResponse>(this);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<int>::Failure(-1);
  }
  return Future<int>(response);
}
