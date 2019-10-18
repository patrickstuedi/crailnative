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

#ifndef NAMENODE_CLIENT_H
#define NAMENODE_CLIENT_H

#include <string>

#include "crail/client/common/future.h"
#include "crail/client/metadata/filename.h"
#include "crail/client/namenode/create_response.h"
#include "crail/client/namenode/getblock_response.h"
#include "crail/client/namenode/ioctl_response.h"
#include "crail/client/namenode/lookup_response.h"
#include "crail/client/namenode/remove_response.h"
#include "crail/client/namenode/void_response.h"

class NamenodeClient {
public:
  // Control ops
  virtual int Connect() = 0;
  virtual void Close() = 0;

  // RPC calls
  virtual Future<CreateResponse> Create(Filename &name, int type,
                                        int storage_class, int location_class,
                                        int enumerable) = 0;
  virtual Future<LookupResponse> Lookup(Filename &name) = 0;
  virtual Future<GetblockResponse> GetBlock(long long fd, long long token,
                                            long long position,
                                            long long capacity) = 0;
  virtual Future<VoidResponse> SetFile(FileInfo &file_info, bool close) = 0;
  virtual Future<RemoveResponse> Remove(Filename &name, bool recursive) = 0;
  virtual Future<IoctlResponse> Ioctl(unsigned char op, Filename &name) = 0;
};

#endif /* NAMENODE_CLIENT_H */
