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

#ifndef CRAIL_NODE_H
#define CRAIL_NODE_H

#include <memory>

#include "common/block_cache.h"
#include "metadata/file_info.h"
#include "namenode/namenode_client.h"
#include "storage/storage_cache.h"

using namespace std;
using namespace crail;

class CrailNode {
public:
  CrailNode() = default;
  CrailNode(shared_ptr<FileInfo> file_info,
            shared_ptr<NamenodeClient> namenode_client,
            shared_ptr<StorageCache> storage_cache,
            shared_ptr<BlockCache> block_cache);
  CrailNode(CrailNode &&node) = default;
  CrailNode(const CrailNode &node) = default;
  virtual ~CrailNode() = default;

  bool valid() const { return file_info_->fd() >= 0; }
  int type() const { return file_info_->type(); }
  unsigned long long fd() const { return file_info_->fd(); }
  unsigned long long capacity() const { return file_info_->capacity(); }

protected:
  shared_ptr<FileInfo> file_info_;
  shared_ptr<NamenodeClient> namenode_client_;
  shared_ptr<StorageCache> storage_cache_;
  shared_ptr<BlockCache> block_cache_;
};

#endif /* CRAIL_NODE_H */
