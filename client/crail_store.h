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

#ifndef CRAIL_STORE_H
#define CRAIL_STORE_H

#include <memory>
#include <string>

#include "common/future.h"
#include "crail_inputstream.h"
#include "crail_node.h"
#include "crail_outputstream.h"
#include "namenode/namenode_client.h"
#include "post_create.h"
#include "storage/storage_cache.h"

using namespace std;
using namespace crail;

namespace crail {

enum class FileType { File = 0, Directory = 1 };

class CrailStore {
public:
  CrailStore();
  virtual ~CrailStore();

  int Initialize(string address, int port);

  template <typename T> T CreateNode(shared_ptr<FileInfo> file_info) {
    shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info->fd());
    T node(file_info, namenode_client_, storage_cache_, file_block_cache);
    return node;
  }

  template <typename T> class PostCreate2 : public AsyncTask<T> {
  public:
    PostCreate2(Future<CreateResponse> future, CrailStore *store,
                Filename filename)
        : future_(future), store_(store), filename_(filename) {}

    T get() {
      auto create_res = future_.get();

      if (create_res.error() != 0) {
        return T();
      }

      auto file_info = create_res.file();
      store_->AddBlock(file_info->fd(), 0, create_res.file_block());

      long long dir_offset = file_info->dir_offset();
      if (dir_offset >= 0) {
        auto parent_info = create_res.parent();
        store_->AddBlock(parent_info->fd(), dir_offset,
                         create_res.parent_block());
        string _name = filename_.name();
        store_->WriteDirectoryRecord(parent_info, _name, dir_offset, 1);
      }

      shared_ptr<BlockCache> file_block_cache =
          store_->GetBlockCache(file_info->fd());
      return store_->CreateNode<T>(file_info);
    }

  private:
    Future<CreateResponse> future_;
    CrailStore *store_;
    Filename filename_;
  };

  template <class T>
  Future<T> Create(string &name, FileType type, int storage_class,
                   int location_class, bool enumerable) {
    Filename filename(name);
    int _enumerable = enumerable ? 1 : 0;
    auto future =
        namenode_client_->Create(filename, static_cast<int>(type),
                                 storage_class, location_class, _enumerable);

    shared_ptr<PostCreate2<T>> post_create =
        std::make_shared<PostCreate2<T>>(future, this, filename);
    return Future<T>(post_create);

    /*
auto create_res = future.get();

if (create_res.error() != 0) {
return nullopt;
}

auto file_info = create_res.file();
AddBlock(file_info->fd(), 0, create_res.file_block());

long long dir_offset = file_info->dir_offset();
if (dir_offset >= 0) {
auto parent_info = create_res.parent();
AddBlock(parent_info->fd(), dir_offset, create_res.parent_block());
string _name = filename.name();
WriteDirectoryRecord(parent_info, _name, dir_offset, 1);
}

shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info->fd());
return T(file_info, namenode_client_, storage_cache_, file_block_cache);
    */
  }

  /*
    template <class T> T PostCreate(Future<CreateResponse> future, string &name)
    { auto create_res = future.get();

      if (create_res.error() != 0) {
        return T();
      }

      auto file_info = create_res.file();
      AddBlock(file_info->fd(), 0, create_res.file_block());

      long long dir_offset = file_info->dir_offset();
      if (dir_offset >= 0) {
        auto parent_info = create_res.parent();
        AddBlock(parent_info->fd(), dir_offset, create_res.parent_block());
        Filename filename(name);
        string _name = filename.name();
        WriteDirectoryRecord(parent_info, _name, dir_offset, 1);
      }

      shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info->fd());
      return T(file_info, namenode_client_, storage_cache_, file_block_cache);
    }
  */

  template <class T> optional<T> Lookup(string &name) {
    Filename filename(name);
    auto future = namenode_client_->Lookup(filename);

    LookupResponse lookup_res = future.get();

    if (lookup_res.error() != 0) {
      return nullopt;
    }

    auto file_info = lookup_res.file();
    AddBlock(file_info->fd(), 0, lookup_res.file_block());
    shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info->fd());
    return T(file_info, namenode_client_, storage_cache_, file_block_cache);
  }

  int Remove(string &name, bool recursive);
  int Ioctl(unsigned char op, string &name);

private:
  unique_ptr<CrailNode> DispatchType(shared_ptr<FileInfo> file_info);
  shared_ptr<BlockCache> GetBlockCache(int fd);
  int AddBlock(int fd, long long offset, shared_ptr<BlockInfo> block);
  unique_ptr<CrailOutputstream> DirectoryOuput(shared_ptr<FileInfo> file_info,
                                               long long position);
  int WriteDirectoryRecord(shared_ptr<FileInfo> directory, string &fname,
                           long long offset, int valid);

  shared_ptr<NamenodeClient> namenode_client_;
  shared_ptr<StorageCache> storage_cache_;
  unordered_map<int, shared_ptr<BlockCache>> block_cache_;
};
} // namespace crail

#endif /* CRAIL_STORE_H */
