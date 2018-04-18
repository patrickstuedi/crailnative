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

#include "reflex_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common/byte_buffer.h"
#include "common/crail_constants.h"
#include "crail_store.h"

using namespace std;
using namespace crail;

ReflexClient::ReflexClient() : isConnected(false), buf_(1024) {
  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  this->counter_ = 1;
}

ReflexClient::~ReflexClient() { Close(); }

int ReflexClient::Connect(int address, int port) {
  if (isConnected) {
    return 0;
  }

  int yes = 1;
  setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(int));

  struct sockaddr_in addr_;
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  memset(&(addr_.sin_zero), 0, 8);
  addr_.sin_addr.s_addr = address;

  if (connect(socket_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
    perror("cannot connect to server");
    return -1;
  }
  isConnected = true;
  return 0;
}

int ReflexClient::Close() {
  if (isConnected) {
    close(socket_);
    isConnected = false;
  }
  return 0;
}

void ReflexClient::Debug(int address, int port) {
  cout << "connecting to ";
  int tmp = address;
  unsigned char *_tmp = (unsigned char *)&tmp;
  for (int i = 0; i < 4; i++) {
    unsigned int ch = (unsigned int)_tmp[i];
    cout << ch << ".";
  }
  cout << ", port " << port << endl;
}

int ReflexClient::IssueRequest(ReflexMessage &request,
                               shared_ptr<ReflexMessage> response) {
  unsigned long long ticket = counter_++;
  responseMap.insert({ticket, response});
  buf_.Clear();

  // narpc header (size, ticket)
  AddNaRPCHeader(buf_, request.Size(), ticket);
  // create file request
  request.Write(buf_);

  // issue request
  buf_.Flip();
  if (SendBytes(buf_.get_bytes(), buf_.remaining()) < 0) {
    return -1;
  }

  shared_ptr<ByteBuffer> payload = request.Payload();
  if (payload) {
    if (SendBytes(payload->get_bytes(), payload->remaining()) < 0) {
      return -1;
    }
  }
  return 0;
}

int ReflexClient::PollResponse() {
  // recv resp header
  buf_.Clear();
  if (RecvBytes(buf_.get_bytes(), kNarpcHeader) < 0) {
    return -1;
  }
  int size = buf_.GetInt();
  long long ticket = buf_.GetLong();

  shared_ptr<ReflexMessage> response = responseMap[ticket];
  responseMap.erase(ticket);

  shared_ptr<ByteBuffer> payload = response->Payload();
  int payload_size = 0;
  if (payload) {
    payload_size = payload->remaining();
  }

  // recv resp obj
  buf_.Clear();
  int header_size = size - payload_size;
  if (RecvBytes(buf_.get_bytes(), header_size) < 0) {
    return -1;
  }

  response->Update(buf_);

  if (payload) {
    if (RecvBytes(payload->get_bytes(), payload->remaining()) < 0) {
      return -1;
    }
  }

  return 0;
}

void ReflexClient::AddNaRPCHeader(ByteBuffer &buf, int size,
                                  unsigned long long ticket) {
  buf.PutInt(size);
  buf.PutLong(ticket);
}

long long ReflexClient::RemoveNaRPCHeader(ByteBuffer &buf) {
  buf.GetInt();
  long long ticket = buf.GetLong();
  return ticket;
}

int ReflexClient::SendBytes(unsigned char *buf, int size) {
  int res = send(socket_, buf, (size_t)size, (int)0);
  if (res < 0) {
    return res;
  }
  int remaining = size - res;
  while (remaining > 0) {
    int offset = size - remaining;
    res = send(socket_, buf + offset, (size_t)remaining, (int)0);
    if (res < 0) {
      return res;
    }
    remaining -= res;
  }
  return remaining;
}

int ReflexClient::RecvBytes(unsigned char *buf, int size) {
  int res = recv(socket_, buf, (size_t)size, (int)0);
  if (res < 0) {
    return res;
  }
  int remaining = size - res;
  while (remaining > 0) {
    int offset = size - remaining;
    res = recv(socket_, buf + offset, (size_t)remaining, (int)0);
    if (res < 0) {
      return res;
    }
    remaining -= res;
  }
  return remaining;
}