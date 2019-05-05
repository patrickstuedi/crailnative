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

#include "narpc/rpc_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

RpcClient::RpcClient(int address, int port, bool nodelay)
    : stream_(address, port, nodelay) {
  this->counter_ = 1;
}

RpcClient::~RpcClient() { Close(); }

int RpcClient::Close() { stream_.Close(); }

int RpcClient::IssueRequest(RpcMessage &request, RpcMessage &response) {
  unsigned long long ticket = counter_++ % RpcClient::kMaxTicket;
  if (ticket == 0) {
    ticket++;
  }
  responseMap_[ticket] = response;
  buf_.Clear();

  // narpc header (size, ticket)
  shared_ptr<Serializable> header = request.Header();
  AddNaRPCHeader(buf_, header->Size(), ticket);
  // create file request
  header->Write(buf_);

  // issue request
  buf_.Flip();
  // int _metadata = buf_.remaining();
  if (SendBytes(buf_.get_bytes(), buf_.remaining()) < 0) {
    cout << "Error when sending rpc message " << endl;
    return -1;
  }

  shared_ptr<ByteBuffer> payload = request.Payload();
  // int _data = 0;
  if (payload) {
    //_data = payload->remaining();
    if (SendBytes(payload->get_bytes(), payload->remaining()) < 0) {
      cout << "Error when sending RPC payload" << endl;
      return -1;
    }
    payload->set_position(payload->position() + payload->remaining());
  }

  // int _total = _metadata + _data;
  // cout << "transmitting message, port " << port_ << ", size " << _total <<
  // endl;

  return 0;
}

int RpcClient::PollResponse() {
  // recv resp header
  buf_.Clear();
  if (RecvBytes(buf_.get_bytes(), kNarpcHeader) < 0) {
    cout << "Error receiving rpc header" << endl;
    return -1;
  }
  int size = buf_.GetInt();
  long long ticket = buf_.GetLong();

  RpcMessage &response = responseMap_[ticket];
  // responseMap_[ticket] = nullptr;

  shared_ptr<ByteBuffer> payload = response.Payload();
  int payload_size = 0;
  if (payload) {
    payload_size = payload->remaining();
  }

  // recv resp obj
  buf_.Clear();
  int header_size = size - payload_size;
  if (RecvBytes(buf_.get_bytes(), header_size) < 0) {
    cout << "Error receiving rpc message" << endl;
    return -1;
  }

  shared_ptr<Serializable> header = response.Header();
  header->Update(buf_);

  if (payload) {
    if (RecvBytes(payload->get_bytes(), payload->remaining()) < 0) {
      cout << "Error receiving rpc payload" << endl;
      return -1;
    }
    payload->set_position(payload->position() + payload->remaining());
  }

  // int _total = kNarpcHeader + size;
  // cout << "receiving message, port " << port_ << ", size " << _total << endl;

  return 0;
}

void RpcClient::AddNaRPCHeader(ByteBuffer &buf, int size,
                               unsigned long long ticket) {
  buf.PutInt(size);
  buf.PutLong(ticket);
}

long long RpcClient::RemoveNaRPCHeader(ByteBuffer &buf) {
  buf.GetInt();
  long long ticket = buf.GetLong();
  return ticket;
}
