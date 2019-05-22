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

#ifndef NARPC_NAMENODE_CLIENT_H
#define NARPC_NAMENODE_CLIENT_H

#include <atomic>
#include <string>

#include "crail/client/common/future.h"
#include "crail/client/metadata/filename.h"
#include "crail/client/namenode/create_response.h"
#include "crail/client/namenode/getblock_response.h"
#include "crail/client/namenode/ioctl_response.h"
#include "crail/client/namenode/lookup_response.h"
#include "crail/client/namenode/namenode_client.h"
#include "crail/client/namenode/remove_response.h"
#include "crail/client/namenode/void_response.h"
#include "narpc/rpc_client.h"

class NarpcNamenodeClient : public NamenodeClient {
public:
  static const bool kNodelay = true;

  NarpcNamenodeClient(int address, int port);
  ~NarpcNamenodeClient();

  int Connect() { return rpc_client_.Connect(); }
  void Close() { rpc_client_.Close(); }

  Future<CreateResponse> Create(Filename &name, int type, int storage_class,
                                int location_class, int enumerable);
  Future<LookupResponse> Lookup(Filename &name);
  Future<GetblockResponse> GetBlock(long long fd, long long token,
                                    long long position, long long capacity);
  Future<VoidResponse> SetFile(FileInfo &file_info, bool close);
  Future<RemoveResponse> Remove(Filename &name, bool recursive);
  Future<IoctlResponse> Ioctl(unsigned char op, Filename &name);

private:
  RpcClient rpc_client_;
};

#endif /* NARPC_NAMENODE_CLIENT_H */
