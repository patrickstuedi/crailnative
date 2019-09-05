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

#include "narpc/network_stream.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "crail/client/common/crail_constants.h"
#include "narpc/network_utils.h"

NetworkStream::NetworkStream() : bytes_(0), metadata_(kBufferSize * 2) {}

NetworkStream::~NetworkStream() {}

void NetworkStream::Clear() {
  metadata_.Clear();
  data_.clear();
}

void NetworkStream::PutByte(unsigned char value) { metadata_.PutByte(value); }

void NetworkStream::PutInt(int value) { metadata_.PutInt(value); }

void NetworkStream::PutShort(short value) { metadata_.PutShort(value); }

void NetworkStream::PutLong(long long value) { metadata_.PutLong(value); }

void NetworkStream::PutData(shared_ptr<ByteBuffer> buffer) {
  data_.push_back(buffer);
  // metadata_.PutBytes((const char *)buffer->get_bytes(), buffer->remaining());
}

void NetworkStream::PutBytes(const char buf[], int length) {
  metadata_.PutBytes(buf, length);
}

unsigned char NetworkStream::GetByte() { return metadata_.GetByte(); }

short NetworkStream::GetShort() { return metadata_.GetShort(); }

int NetworkStream::GetInt() { return metadata_.GetInt(); }

long long NetworkStream::GetLong() { return metadata_.GetLong(); }

void NetworkStream::GetBytes(char buf[], int length) {
  return metadata_.GetBytes(buf, length);
}

int NetworkStream::Write(int socket) {
  metadata_.Flip();
  unsigned char *buf = metadata_.get_bytes();
  int size = metadata_.remaining();

  /*
SendBytes(socket, buf, size);

for (shared_ptr<ByteBuffer> databuf : data_) {
SendBytes(socket, databuf->get_bytes(), databuf->remaining());
}
  */

  int vec_index = 0;
  struct iovec iov[4];
  iov[vec_index].iov_base = buf;
  iov[vec_index].iov_len = size;
  vec_index++;

  for (shared_ptr<ByteBuffer> databuf : data_) {
    iov[vec_index].iov_base = databuf->get_bytes();
    iov[vec_index].iov_len = databuf->remaining();
    vec_index++;
  }

  SendBytesV(socket, iov, vec_index);

  return 0;
}

int NetworkStream::SendBytesV(int socket, struct iovec *iov, int vec_count) {
  return writev(socket, iov, vec_count);
}

int NetworkStream::SendBytes(int socket, unsigned char *buf, int size) {

  cout << "sending buf of size " << size << endl;
  int res = send(socket, buf, (size_t)size, (int)0);
  cout << "res " << res << endl;
  if (res < 0) {
    return res;
  }
  int remaining = size - res;
  while (remaining > 0) {
    int offset = size - remaining;
    res = send(socket, buf + offset, (size_t)remaining, (int)0);
    if (res < 0) {
      return res;
    }
    remaining -= res;
  }
  return remaining;
}

int NetworkStream::Read(int socket, int size) {
  unsigned char *buf = metadata_.get_bytes();
  int old_pos = metadata_.position();

  int sum = 0;
  while (sum < size) {
    int res = recv(socket, buf + sum, (size_t)(size - sum), MSG_DONTWAIT);

    if (res < 0) {
      if (errno == EAGAIN) {
        continue;
      }
      // return res;
      break;
    }

    sum += res;
  }

  metadata_.set_position(old_pos + sum);
  metadata_.Flip();
  metadata_.set_position(old_pos);

  cout << "after read, pos " << metadata_.position() << ", limit "
       << metadata_.limit() << endl;

  return sum;
}
/*
int NetworkStream::Write(unsigned char *buf, int size) {
  iov[vec_count_].iov_base = buf;
  iov[vec_count_].iov_len = size;
  vec_count_++;
  bytes_ += size;

  return 0;
}

int NetworkStream::Read(unsigned char *buf, int size) {
  iov[vec_count_].iov_base = buf;
  iov[vec_count_].iov_len = size;
  vec_count_++;
  bytes_ += size;

  return 0;
}

void NetworkStream::Sync() {
  int ret = readv(socket_, iov, vec_count_);
  cout << "Syncing iovec, count " << vec_count_ << ", ret " << ret << endl;
  vec_count_ = 0;
  bytes_ = 0;
}

void NetworkStream::Flush() {
  cout << "Flushing iovec, count " << vec_count_ << ", bytes " << bytes_
       << endl;
  writev(socket_, iov, vec_count_);
  vec_count_ = 0;
  bytes_ = 0;
}
*/
