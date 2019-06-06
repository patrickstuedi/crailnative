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

#include "narpc/network_utils.h"

NetworkStream::NetworkStream(int address, int port, bool nodelay)
    : isConnected(false), address_(address), port_(port), nodelay_(nodelay),
      vec_count_(0), bytes_(0) {
  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
}

NetworkStream::~NetworkStream() {}

int NetworkStream::Connect() {
  if (isConnected) {
    return 0;
  }

  int yes = 0;
  if (nodelay_) {
    yes = 1;
  }
  setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(int));

  struct sockaddr_in addr_;
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port_);
  memset(&(addr_.sin_zero), 0, 8);
  addr_.sin_addr.s_addr = address_;

  string addressport = NetworkUtils::GetAddress(address_, port_);
  if (connect(socket_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
    string message =
        "NetworkStream::Connect cannot connect to server, " + addressport;
    perror(message.c_str());
    return -1;
  } else {
    cout << "NetworkStream::Connect connected to " << addressport << endl;
  }
  isConnected = true;
  return 0;
}

void NetworkStream::Close() {
  if (!isConnected) {
    return;
  }
  isConnected = false;
  ::close(socket_);
}

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
