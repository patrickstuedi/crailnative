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

#ifndef VOID_RESPONSE_H
#define VOID_RESPONSE_H

#include "crail/client/namenode/namenode_response.h"

class VoidResponse : public NamenodeResponse {
public:
  VoidResponse();
  virtual ~VoidResponse();

  virtual int UpdateMetedata(ByteBuffer &buffer);

  int Size() const { return NamenodeResponse::Size(); }
  int Write(NetworkStream &stream) const;
  int Update(NetworkStream &stream);
  string ToString() const { return "VoidResponse:: " + to_string(error_); }

private:
  short error_;
};

#endif /* VOID_RESPONSE_H */
