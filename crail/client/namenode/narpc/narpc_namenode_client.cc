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

#include "crail/client/namenode/narpc/narpc_namenode_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "crail/client/crail_store.h"
#include "crail/client/namenode/create_request.h"
#include "crail/client/namenode/getblock_request.h"
#include "crail/client/namenode/ioctl_request.h"
#include "crail/client/namenode/lookup_request.h"
#include "crail/client/namenode/narpc/narpc_create_response.h"
#include "crail/client/namenode/narpc/narpc_getblock_response.h"
#include "crail/client/namenode/narpc/narpc_ioctl_response.h"
#include "crail/client/namenode/narpc/narpc_lookup_response.h"
#include "crail/client/namenode/narpc/narpc_remove_response.h"
#include "crail/client/namenode/narpc/narpc_void_response.h"
#include "crail/client/namenode/remove_request.h"
#include "crail/client/namenode/setfile_request.h"

NarpcNamenodeClient::NarpcNamenodeClient(int address, int port)
    : rpc_client_(address, port, NarpcNamenodeClient::kNodelay) {}

NarpcNamenodeClient::~NarpcNamenodeClient() {}

Future<CreateResponse> NarpcNamenodeClient::Create(Filename &name, int type,
                                                   int storage_class,
                                                   int location_class,
                                                   int enumerable) {
  shared_ptr<CreateRequest> request = make_shared<CreateRequest>(
      name, type, storage_class, location_class, enumerable);
  shared_ptr<NarpcCreateResponse> response =
      make_shared<NarpcCreateResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<CreateResponse>::Failure();
  }
  return Future<CreateResponse>(response);
}

Future<LookupResponse> NarpcNamenodeClient::Lookup(Filename &name) {
  shared_ptr<LookupRequest> request = make_shared<LookupRequest>(name);
  shared_ptr<NarpcLookupResponse> response =
      make_shared<NarpcLookupResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<LookupResponse>::Failure();
  }
  return Future<LookupResponse>(response);
}

Future<GetblockResponse> NarpcNamenodeClient::GetBlock(long long fd,
                                                       long long token,
                                                       long long position,
                                                       long long capacity) {
  shared_ptr<GetblockRequest> request =
      make_shared<GetblockRequest>(fd, token, position, capacity);
  shared_ptr<NarpcGetBlockResponse> response =
      make_shared<NarpcGetBlockResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<GetblockResponse>::Failure();
  }
  return Future<GetblockResponse>(response);
}

Future<VoidResponse> NarpcNamenodeClient::SetFile(FileInfo &file_info,
                                                  bool close) {
  shared_ptr<SetfileRequest> request =
      make_shared<SetfileRequest>(file_info, close);
  shared_ptr<NarpcVoidResponse> response =
      make_shared<NarpcVoidResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<VoidResponse>::Failure();
  }
  return Future<VoidResponse>(response);
}

Future<RemoveResponse> NarpcNamenodeClient::Remove(Filename &name,
                                                   bool recursive) {
  shared_ptr<RemoveRequest> request =
      make_shared<RemoveRequest>(name, recursive);
  shared_ptr<NarpcRemoveResponse> response =
      make_shared<NarpcRemoveResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<RemoveResponse>::Failure();
  }
  return Future<RemoveResponse>(response);
}

Future<IoctlResponse> NarpcNamenodeClient::Ioctl(unsigned char op,
                                                 Filename &name) {
  shared_ptr<IoctlRequest> request = make_shared<IoctlRequest>(op, name);
  shared_ptr<NarpcIoctlResponse> response =
      make_shared<NarpcIoctlResponse>(&rpc_client_);

  if (rpc_client_.IssueRequest(request, response) < 0) {
    return Future<IoctlResponse>::Failure();
  }
  return Future<IoctlResponse>(response);
}
