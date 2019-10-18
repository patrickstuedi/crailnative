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

#ifndef CRAIL_FILE_H
#define CRAIL_FILE_H

#include "crail/client/common/block_cache.h"
#include "crail/client/common/file_type.h"
#include "crail/client/crail_inputstream.h"
#include "crail/client/crail_node.h"
#include "crail/client/crail_outputstream.h"
#include "crail/client/metadata/file_info.h"
#include "crail/client/storage/storage_cache.h"

using namespace std;

class CrailFile : public CrailNode {
public:
  static const FileType type = FileType::File;

  CrailFile() = default;
  CrailFile(FileInfo file_info, shared_ptr<NamenodeClient> namenode_client,
            shared_ptr<StorageCache> storage_cache,
            shared_ptr<BlockCache> block_cache);
  virtual ~CrailFile() = default;

  unique_ptr<CrailOutputstream> outputstream();
  unique_ptr<CrailInputstream> inputstream();
};

#endif /* CRAIL_FILE_H */
