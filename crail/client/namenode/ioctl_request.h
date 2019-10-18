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

#ifndef IOCTL_REQUEST_H
#define IOCTL_REQUEST_H

#include "crail/client/common/serializable.h"
#include "crail/client/metadata/filename.h"
#include "crail/client/namenode/namenode_request.h"
#include "ioutils/byte_buffer.h"

using namespace crail;

class IoctlRequest : public NamenodeRequest {
public:
  IoctlRequest(unsigned char op, Filename &name);
  virtual ~IoctlRequest();

  virtual int Write(ByteBuffer &buffer) { return 0; }

  int Size() const {
    return NamenodeRequest::Size() + sizeof(op_) + filename_.Size();
  }

  const Filename &filename() const { return filename_; }

private:
  unsigned char op_;
  Filename filename_;
};

#endif /* IOCTL_REQUEST_H */
