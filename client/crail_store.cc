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

#include "crail_store.h"

#include <arpa/inet.h>
#include <iostream>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common/crail_constants.h"
#include "crail_directory.h"
#include "crail_file.h"
#include "directory_record.h"
#include "metadata/filename.h"
#include "storage/storage_client.h"
#include "storage/write_request.h"
#include "storage/write_response.h"

using namespace crail;

CrailStore::CrailStore() : namenode_client_(new NamenodeClient()) {}

CrailStore::~CrailStore() {}

int CrailStore::Initialize(string address, int port) {
  return this->namenode_client_->Connect((int)inet_addr(address.c_str()), port);
}

unique_ptr<CrailNode> CrailStore::Create(string &name, FileType type) {
  Filename filename(name);
  auto create_res =
      namenode_client_->Create(filename, static_cast<int>(type), 0, 0, 1);

  if (create_res->file()->dir_offset() >= 0) {
    auto directory_stream = make_unique<CrailOutputstream>(
        this->namenode_client_, create_res->parent(),
        create_res->file()->dir_offset());
    string fname = filename.name();
    DirectoryRecord record(1, fname);
    shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(1024);
    record.Write(*buf);
    buf->Flip();
    directory_stream->Write(buf);
  }

  auto file_info = create_res->file();
  return DispatchType(file_info);
}

unique_ptr<CrailNode> CrailStore::Lookup(string &name) {
  Filename filename(name);
  auto lookup_res = namenode_client_->Lookup(filename);
  auto file_info = lookup_res->file();

  return DispatchType(file_info);
}

int CrailStore::Remove(string &name) { return 0; }

unique_ptr<CrailNode> CrailStore::DispatchType(shared_ptr<FileInfo> file_info) {
  if (file_info->type() == static_cast<int>(FileType::File)) {
    return make_unique<CrailFile>(file_info, namenode_client_);
  } else if (file_info->type() == static_cast<int>(FileType::Directory)) {
    return make_unique<CrailDirectory>(file_info, namenode_client_);
  } else {
    return nullptr;
  }
}
