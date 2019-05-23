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

#ifndef CREATE_RESPONSE_H
#define CREATE_RESPONSE_H

#include <memory>

#include "crail/client/common/serializable.h"
#include "crail/client/metadata/block_info.h"
#include "crail/client/metadata/file_info.h"
#include "crail/client/namenode/namenode_response.h"

using namespace std;

class NamenodeClient;

class CreateResponse : public NamenodeResponse {
public:
  CreateResponse();
  virtual ~CreateResponse();

  virtual string Name() const { return "CreateResponse"; }
  int Size() const {
    return NamenodeResponse::Size() + file_info_.Size() * 2 +
           file_block_->Size() * 2;
  }
  int Write(NetworkStream &stream) const;
  int Update(NetworkStream &stream);

  FileInfo file() const { return file_info_; }
  FileInfo parent() const { return parent_info_; }
  shared_ptr<BlockInfo> file_block() const { return file_block_; }
  shared_ptr<BlockInfo> parent_block() const { return parent_block_; }

private:
  FileInfo file_info_;
  FileInfo parent_info_;
  shared_ptr<BlockInfo> file_block_;
  shared_ptr<BlockInfo> parent_block_;
  ByteBuffer buffer_;
};

#endif /* CREATE_RESPONSE_H */
