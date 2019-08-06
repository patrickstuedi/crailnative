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

#ifndef STORAGE_CLIENT_H
#define STORAGE_CLIENT_H

#include "crail/client/common/future.h"
#include "ioutils/byte_buffer.h"

using namespace crail;
using namespace std;

class StorageClient {
public:
  virtual int Connect() = 0;
  virtual void Close() = 0;

  virtual Future<int> WriteData(int key, long long address,
                                shared_ptr<ByteBuffer> buf) = 0;
  virtual Future<int> ReadData(int key, long long address,
                               shared_ptr<ByteBuffer> buf) = 0;
};

#endif /* STORAGE_CLIENT_H */
