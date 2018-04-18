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

#ifndef WRITE_RESPONSE_H
#define WRITE_RESPONSE_H

#include <memory>

#include "narpc/rpc_message.h"
#include "storage_response.h"

using namespace std;

class WriteResponse : public StorageResponse, public RpcMessage {
public:
  WriteResponse();
  virtual ~WriteResponse();

  shared_ptr<ByteBuffer> Payload() { return nullptr; }

  int Size() const { return sizeof(int); }
  int Write(ByteBuffer &buf) const;
  int Update(ByteBuffer &buf);

private:
  int ret_;
};

#endif /* WRITE_RESPONSE_H */