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

#ifndef FUTURE_H
#define FUTURE_H

#include <memory>

using namespace std;

template <typename T> class AsyncTask {
public:
  virtual T get() = 0;
};

template <typename T> class Future : public AsyncTask<T> {
public:
  Future(shared_ptr<AsyncTask<T>> task) : task_(task), is_done_(false) {}

  virtual ~Future<T>() {}

  static Future Failure() { return Future(); }
  static Future Failure(T val) { return Future(val); }

  virtual T get() {
    if (!is_done_ && task_ != nullptr) {
      result_ = task_->get();
      is_done_ = true;
    }
    return result_;
  }

private:
  Future() : task_(nullptr), is_done_(true) {}
  Future(T result) : task_(nullptr), is_done_(true) { result_ = result; }

  shared_ptr<AsyncTask<T>> task_;
  T result_;
  bool is_done_;
};

#endif /* FUTURE_H */
