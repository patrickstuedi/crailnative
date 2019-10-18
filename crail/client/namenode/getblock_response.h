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

#ifndef GETBLOCK_RESPONSE_H
#define GETBLOCK_RESPONSE_H

#include "crail/client/metadata/block_info.h"
#include "crail/client/namenode/namenode_response.h"

using namespace std;

class GetblockResponse : public NamenodeResponse {
public:
  GetblockResponse();
  virtual ~GetblockResponse();

  virtual int UpdateMetedata(ByteBuffer &buffer) { return 0; }
  virtual int WriteMetadata(ByteBuffer &buffer) { return 0; }
  virtual shared_ptr<ByteBuffer> GetPayload() { return nullptr; }

  int Size() const {
    return NamenodeResponse::Size() + block_info_.Size() + sizeof(short);
  }

  BlockInfo &block_info() { return block_info_; }

private:
  BlockInfo block_info_;
  short error_;
};

#endif /* GETBLOCK_RESPONSE_H */
