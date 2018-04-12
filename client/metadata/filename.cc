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

#include "filename.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "common/crail_utils.h"

using namespace crail;

Filename::Filename(string &name) {
  std::string token;
  std::stringstream tokenStream(name);
  int index = 0;
  while (std::getline(tokenStream, token, '/')) {
    if (token.empty()) {
      continue;
    }
    components_[index] = file_hash(token);
    name_.push_back(token);
    index++;
  }
  length_ = index;
  while (index < kDirectoryDepth) {
    components_[index] = 0;
    index++;
  }
}

Filename::~Filename() {}

int Filename::Write(ByteBuffer &buf) const {
  buf.PutInt(length_);
  for (int i = 0; i < kDirectoryDepth; i++) {
    buf.PutInt(components_[i]);
  }

  return Size();
}

int Filename::Update(ByteBuffer &buf) {
  length_ = buf.GetInt();
  for (int i = 0; i < kDirectoryDepth; i++) {
    components_[i] = buf.GetInt();
  }
  return Size();
}

int Filename::Size() const { return 4 + 16 * 4; }
