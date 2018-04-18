/*
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

#ifndef STORAGE_CACHE_H
#define STORAGE_CACHE_H

#include "storage_client.h"
#include <map>
#include <memory>

using namespace std;
using namespace crail;

class StorageCache {
public:
  StorageCache();
  virtual ~StorageCache();

  int Put(long long key, shared_ptr<StorageClient> endpoint);
  shared_ptr<StorageClient> Get(long long key, int storage_class);

private:
  map<long long, shared_ptr<StorageClient>> cache_;
};

#endif /* STORAGE_CACHE_H */
