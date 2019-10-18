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

#include "crail/client/namenode/create_response.h"

CreateResponse::CreateResponse()
    : NamenodeResponse(), file_block_(new BlockInfo()),
      parent_block_(new BlockInfo()) {}

CreateResponse::~CreateResponse() {}

int CreateResponse::Update(ByteBuffer &buffer) {
  NamenodeResponse::Update(buffer);

  file_info_.Update(buffer);
  parent_info_.Update(buffer);
  file_block_->Update(buffer);
  parent_block_->Update(buffer);

  return 0;
}
