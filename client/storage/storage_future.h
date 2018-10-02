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

#ifndef STORAGE_FUTURE_H
#define STORAGE_FUTURE_H

#include "common/future.h"

template <typename T> class StorageFuture : public Future<T> {
public:
  StorageFuture(shared_ptr<RpcResponse> rpc_response, T result)
      : Future<T>(result), rpc_response_(rpc_response) {
    request_failed_ = false;
  }

  virtual ~StorageFuture<T>() {}

  T get() {
    if (!request_failed_) {
      rpc_response_->Get();
    }
    return Future<T>::get();
  }

  void request_failed() { request_failed_ = true; }

private:
  bool request_failed_;
  shared_ptr<RpcResponse> rpc_response_;
};

#endif /* STORAGE_FUTURE_H */
