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

#ifndef NAMENODE_FUTURE_H
#define NAMENODE_FUTURE_H

#include "common/future.h"

template <typename T> class NamenodeFuture : public Future<T> {
public:
  NamenodeFuture(shared_ptr<RpcResponse> rpc_response, shared_ptr<T> result)
      : Future<T>(*result), rpc_response_(rpc_response),
        actual_result_(result) {}

  virtual ~NamenodeFuture<T>() {}

  T get() {
    rpc_response_->Get();
    return *actual_result_;
  }

private:
  shared_ptr<RpcResponse> rpc_response_;
  shared_ptr<T> actual_result_;
};

#endif /* NAMENODE_FUTURE_H */
