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

#ifndef NARPC_LOOKUP_RESPONSE_H
#define NARPC_LOOKUP_RESPONSE_H

#include "common/future.h"
#include "namenode/lookup_response.h"
#include "namenode/narpc/narpc_namenode_client.h"

class NarpcLookupResponse : public LookupResponse,
                            public AsyncTask<LookupResponse> {
public:
  NarpcLookupResponse(NarpcNamenodeClient *client)
      : LookupResponse(), client_(client) {}
  virtual ~NarpcLookupResponse() {}

  LookupResponse get() { return *this; }

private:
  NarpcNamenodeClient *client_;
};

#endif /* NARPC_LOOKUP_RESPONSE_H */
