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

#include "namenode_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "crail_store.h"
#include "create_request.h"
#include "create_response.h"
#include "getblock_request.h"
#include "getblock_response.h"
#include "ioctl_request.h"
#include "lookup_request.h"
#include "namenode_future.h"
#include "remove_request.h"
#include "setfile_request.h"

NamenodeClient::NamenodeClient() : RpcClient(NamenodeClient::kNodelay) {
  this->counter_ = 1;
}

NamenodeClient::~NamenodeClient() {}

NamenodeFuture<CreateResponse> NamenodeClient::Create(Filename &name, int type,
                                                      int storage_class,
                                                      int location_class,
                                                      int enumerable) {
  Createrequest createReq(name, type, storage_class, location_class,
                          enumerable);
  shared_ptr<CreateResponse> getblockRes = make_shared<CreateResponse>(this);
  NamenodeFuture<CreateResponse> future(getblockRes, getblockRes);
  if (RpcClient::IssueRequest(createReq, getblockRes) < 0) {
    future.request_failed();
  }
  return future;
}

NamenodeFuture<LookupResponse> NamenodeClient::Lookup(Filename &name) {
  LookupRequest lookupReq(name);
  shared_ptr<LookupResponse> lookupRes = make_shared<LookupResponse>(this);
  NamenodeFuture<LookupResponse> future(lookupRes, lookupRes);
  if (RpcClient::IssueRequest(lookupReq, lookupRes) < 0) {
    future.request_failed();
  }
  return future;
}

NamenodeFuture<GetblockResponse> NamenodeClient::GetBlock(long long fd,
                                                          long long token,
                                                          long long position,
                                                          long long capacity) {
  GetblockRequest get_block_req(fd, token, position, capacity);
  shared_ptr<GetblockResponse> get_block_res =
      make_shared<GetblockResponse>(this);
  NamenodeFuture<GetblockResponse> future(get_block_res, get_block_res);
  if (RpcClient::IssueRequest(get_block_req, get_block_res) < 0) {
    future.request_failed();
  }
  return future;
}

NamenodeFuture<VoidResponse>
NamenodeClient::SetFile(shared_ptr<FileInfo> file_info, bool close) {
  SetfileRequest set_file_req(file_info, close);
  shared_ptr<VoidResponse> set_file_res = make_shared<VoidResponse>(this);
  NamenodeFuture<VoidResponse> future(set_file_res, set_file_res);
  if (RpcClient::IssueRequest(set_file_req, set_file_res) < 0) {
    future.request_failed();
  }
  return future;
}

NamenodeFuture<RemoveResponse> NamenodeClient::Remove(Filename &name,
                                                      bool recursive) {
  RemoveRequest remove_req(name, recursive);
  shared_ptr<RemoveResponse> remove_res = make_shared<RemoveResponse>(this);
  NamenodeFuture<RemoveResponse> future(remove_res, remove_res);
  if (RpcClient::IssueRequest(remove_req, remove_res) < 0) {
    future.request_failed();
  }
  return future;
}

NamenodeFuture<IoctlResponse> NamenodeClient::Ioctl(unsigned char op,
                                                    Filename &name) {
  IoctlRequest ioctl_request(op, name);
  shared_ptr<IoctlResponse> ioctl_response = make_shared<IoctlResponse>(this);
  NamenodeFuture<IoctlResponse> future(ioctl_response, ioctl_response);
  if (RpcClient::IssueRequest(ioctl_request, ioctl_response) < 0) {
    future.request_failed();
  }
  return future;
}
