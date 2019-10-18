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

#include "crail/client/storage/storage_cache.h"
#include <iostream>

#include "crail/client/common/crail_constants.h"
#include "crail/client/storage/narpc/narpc_storage_client.h"

using namespace crail;

StorageCache::StorageCache() {}

StorageCache::~StorageCache() {}

void StorageCache::Close() {
  for (std::pair<long long, shared_ptr<StorageClient>> element : cache_) {
    element.second->Close();
  }
}

shared_ptr<StorageClient> StorageCache::Get(DatanodeInfo dn_info) {
  long long key = dn_info.Key();
  auto iter = cache_.find(key);
  if (iter != cache_.end()) {
    return iter->second;
  } else {
    shared_ptr<StorageClient> client = CreateClient(dn_info);
    client->Connect();
    cache_.insert({key, client});
    return client;
  }
}

shared_ptr<StorageClient> StorageCache::CreateClient(DatanodeInfo dn_info) {
  if (dn_info.storage_class() == 0) {
    return make_shared<NarpcStorageClient>(dn_info.addr(), dn_info.port());
  } else {
    return make_shared<NarpcStorageClient>(dn_info.addr(), dn_info.port());
  }
}
