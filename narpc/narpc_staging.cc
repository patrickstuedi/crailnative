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

#include "narpc/narpc_staging.h"

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

NarpcStaging::NarpcStaging() : bytes_(0), metadata_(kBufferSize * 2) {}

NarpcStaging::~NarpcStaging() {}

void NarpcStaging::Clear() {
  metadata_.Clear();
  data_.clear();
}

void NarpcStaging::AddHeader(int size, unsigned long long ticket) {
  metadata_.PutInt(size);
  metadata_.PutLong(ticket);
}

void NarpcStaging::SendMessage(int socket, shared_ptr<RpcMessage> message) {
  message->WriteMetadata(metadata_);
  shared_ptr<ByteBuffer> payload = message->GetPayload();
  if (payload) {
    data_.push_back(payload);
  }
  Flush(socket);
}

int NarpcStaging::FetchHeader(int socket, int &size,
                              unsigned long long &ticket) {
  ReceiveBytes(socket, sizeof(int) * 2, metadata_.get_bytes());

  size = metadata_.GetInt();
  ticket = metadata_.GetLong();

  return 0;
}

int NarpcStaging::FetchMessage(int socket, shared_ptr<RpcMessage> message) {

  int old_pos = metadata_.position();

  int sum = ReceiveBytes(socket, message->Size(), metadata_.get_bytes());

  metadata_.set_position(old_pos + sum);
  metadata_.Flip();
  metadata_.set_position(old_pos);

  cout << "after read, pos " << metadata_.position() << ", limit "
       << metadata_.limit() << endl;

  shared_ptr<ByteBuffer> payload = message->GetPayload();
  if (payload) {
    ReceiveBytes(socket, payload->Size(), payload->get_bytes());
  }

  return 0;
}

int NarpcStaging::Flush(int socket) {
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
  struct iovec iov[1 + data_.size()];

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

int NarpcStaging::SendBytesV(int socket, struct iovec *iov, int vec_count) {
  return writev(socket, iov, vec_count);
}

int NarpcStaging::SendBytes(int socket, unsigned char *buf, int size) {

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

int NarpcStaging::ReceiveBytes(int socket, int size, unsigned char *buf) {

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

  return sum;
}
