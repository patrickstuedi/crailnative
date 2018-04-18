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

#ifndef REFLEX_CLIENT_H
#define REFLEX_CLIENT_H

#include <atomic>
#include <atomic>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

#include "common/byte_buffer.h"
#include "common/serializable.h"
#include "reflex_message.h"

using namespace std;
using namespace crail;

namespace crail {

class ReflexClient {
public:
  ReflexClient();
  virtual ~ReflexClient();

  const int kNarpcHeader = 12;
  const int kRpcHeader = 4;

  int Connect(int address, int port);
  int IssueRequest(ReflexMessage &request, shared_ptr<ReflexMessage> response);
  int PollResponse();
  int Close();

private:
  int SendBytes(unsigned char *buf, int size);
  int RecvBytes(unsigned char *buf, int size);
  void AddNaRPCHeader(ByteBuffer &buf, int size, unsigned long long ticket);
  long long RemoveNaRPCHeader(ByteBuffer &buf);
  void Debug(int address, int port);

  int socket_;
  atomic<unsigned long long> counter_;
  unordered_map<long long, shared_ptr<ReflexMessage>> responseMap;
  bool isConnected;
  ByteBuffer buf_;
};
}

#endif /* REFLEX_CLIENT_H */
