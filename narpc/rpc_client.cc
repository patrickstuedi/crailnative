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

RpcClient::~RpcClient() { stream_.Close(); }

int RpcClient::Connect() { return stream_.Connect(); }

void RpcClient::Close() { stream_.Close(); }

int RpcClient::IssueRequest(shared_ptr<RpcMessage> request,
                            shared_ptr<RpcMessage> response) {
  unsigned long long ticket = counter_++ % RpcClient::kMaxTicket;
  if (ticket == 0) {
    ticket++;
  }
  responseMap_[ticket] = response;

  cout << "RpcClient::IssueRequest, message " << request->Name() << ", size "
       << request->Size() << endl;

  // write narpc header (size, ticket)
  int *_tmpint = (int *)header_;
  *_tmpint = htonl(request->Size());
  _tmpint++;
  long long *_tmplong = (long long *)_tmpint;
  *_tmplong = htobe64(ticket);
  stream_.Write(header_, kNarpcHeader);

  // write actual rpc message
  request->Write(stream_);
  stream_.Flush();

  return 0;
}

int RpcClient::PollResponse() {
  // recv narpc header
  stream_.Read(header_, kNarpcHeader);
  stream_.Sync();
  int *_tmpint = (int *)header_;
  int size = ntohl(*_tmpint);
  _tmpint++;
  long long *_tmplong = (long long *)_tmpint;
  long long ticket = be64toh(*_tmplong);

  cout << "receiving message, ticket " << ticket << ", size " << size << endl;

  // recv message
  shared_ptr<RpcMessage> response = responseMap_[ticket];

  cout << "response message " << response->Name() << endl;
  response->Update(stream_);

  return 0;
}
