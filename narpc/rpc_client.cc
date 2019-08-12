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

#include "narpc/network_utils.h"

using namespace std;

RpcClient::RpcClient(int address, int port, bool nodelay)
    : isConnected_(false), address_(address), port_(port), nodelay_(nodelay) {
  this->counter_ = 1;
  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
}

RpcClient::~RpcClient() { Close(); }

int RpcClient::Connect() {
  if (isConnected_) {
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
  isConnected_ = true;
  return 0;
}

void RpcClient::Close() {
  if (!isConnected_) {
    return;
  }
  isConnected_ = false;
  ::close(socket_);
}

int RpcClient::IssueRequest(shared_ptr<RpcMessage> request,
                            shared_ptr<RpcMessage> response) {
  unsigned long long ticket = counter_++ % RpcClient::kMaxTicket;
  if (ticket == 0) {
    ticket++;
  }
  responseMap_[ticket] = response;

  cout << "RpcClient::IssueRequest " << request->ToString() << " (size "
       << request->Size() << ")" << endl;

  // write narpc header (size, ticket)
  /*
int *_tmpint = (int *)header_;
*_tmpint = htonl(request->Size());
_tmpint++;
long long *_tmplong = (long long *)_tmpint;
*_tmplong = htobe64(ticket);
stream_.Write(header_, kNarpcHeader);
  */

  // serialize header
  stream_.PutInt(request->Size());
  stream_.PutLong(ticket);

  // serialize rpc message
  request->Write(stream_);

  // write message to network
  stream_.Write(socket_);
  // stream_.Flush();

  return 0;
}

int RpcClient::PollResponse() {
  // recv narpc header
  stream_.Read(socket_);

  /*
stream_.Read(header_, kNarpcHeader);
stream_.Sync();
int *_tmpint = (int *)header_;
int size = ntohl(*_tmpint);
_tmpint++;
long long *_tmplong = (long long *)_tmpint;
long long ticket = be64toh(*_tmplong);
  */
  int size = stream_.GetInt();
  long long ticket = stream_.GetLong();

  // recv message
  shared_ptr<RpcMessage> response = responseMap_[ticket];
  response->Update(stream_);
  // stream_.Sync();
  // response->Sync();
  cout << "RpcMessage::PollResponse " << response << " (size "
       << response->Size() << ")" << endl;

  return 0;
}
