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

#ifndef NETWORK_STREAM_H
#define NETWORK_STREAM_H

#include <fcntl.h>
#include <memory>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <vector>

#include "ioutils/byte_buffer.h"

using namespace crail;
using namespace std;

class NetworkStream {
public:
  NetworkStream();
  virtual ~NetworkStream();

  void PutByte(unsigned char value);
  void PutShort(short value);
  void PutInt(int value);
  void PutLong(long long value);
  void PutData(shared_ptr<ByteBuffer> data);
  void PutBytes(const char value[], int length);

  unsigned char GetByte();
  short GetShort();
  int GetInt();
  long long GetLong();
  void GetData(shared_ptr<ByteBuffer> data);
  void GetBytes(char value[], int length);

  void Clear();
  int Write(int socket);
  int Read(int socket, int size);

private:
  /*
struct iovec iov[4];
int vec_count_;
  */

  ByteBuffer metadata_;
  vector<shared_ptr<ByteBuffer>> data_;
  int bytes_;

  int SendBytes(int socket, unsigned char *buf, int length);
  int SendBytesV(int socket, struct iovec *iov, int vec_count);
};

#endif /* NETWORK_STREAM_H */
