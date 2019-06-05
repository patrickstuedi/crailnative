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

#include <iostream>
#include <string>

#include "crail/client/common/future.h"
#include "crail/client/crail_outputstream.h"
#include "crail/client/metadata/filename.h"
#include "crail/client/storage/storage_cache.h"

using namespace std;
using namespace crail;

namespace crail {

class CrailStore {
public:
  CrailStore(string address, int port);
  virtual ~CrailStore();

  void Initialize() { namenode_client_->Connect(); }

  template <class T>
  Future<T> Create(string &name, int storage_class, int location_class,
                   bool enumerable) {
    Filename filename(name);
    auto future =
        namenode_client_->Create(filename, static_cast<int>(T::type),
                                 storage_class, location_class, enumerable);

    shared_ptr<PostCreate<T>> post_create =
        std::make_shared<PostCreate<T>>(this, future, filename);
    return Future<T>(post_create);
  }

  template <class T> Future<T> Lookup(string &name) {
    Filename filename(name);
    auto future = namenode_client_->Lookup(filename);

    LookupResponse lookup_res = future.get();

    shared_ptr<PostLookup<T>> post_lookup =
        std::make_shared<PostLookup<T>>(this, future);
    return Future<T>(post_lookup);
  }

  int Remove(string &name, bool recursive);
  int Ioctl(unsigned char op, string &name);

private:
  template <typename T> class PostCreate : public AsyncTask<T> {
  public:
    PostCreate(CrailStore *store, Future<CreateResponse> future,
               Filename filename)
        : store_(store), future_(future), filename_(filename) {}

    T get() { return store_->_Create<T>(future_, filename_); }

  private:
    CrailStore *store_;
    Future<CreateResponse> future_;
    Filename filename_;
  };

  template <typename T> class PostLookup : public AsyncTask<T> {
  public:
    PostLookup(CrailStore *store, Future<LookupResponse> future)
        : store_(store), future_(future) {}

    T get() { return store_->_Lookup<T>(future_); }

  private:
    CrailStore *store_;
    Future<LookupResponse> future_;
  };

  template <class T>
  T _Create(Future<CreateResponse> future, Filename &filename) {
    auto create_res = future.get();

    if (create_res.error() != 0) {
      return T();
    }

    auto file_info = create_res.file();
    AddBlock(file_info.fd(), 0, create_res.file_block());

    long long dir_offset = file_info.dir_offset();
    if (dir_offset >= 0) {
      auto parent_info = create_res.parent();
      AddBlock(parent_info.fd(), dir_offset, create_res.parent_block());
      string _name = filename.name();
      WriteDirectoryRecord(parent_info, _name, dir_offset, 1);
    }

    shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info.fd());
    return T(file_info, namenode_client_, storage_cache_, file_block_cache);
  }

  template <class T> T _Lookup(Future<LookupResponse> future) {
    auto lookup_res = future.get();

    if (lookup_res.error() != 0) {
      return T();
    }

    auto file_info = lookup_res.file();
    AddBlock(file_info.fd(), 0, lookup_res.file_block());
    shared_ptr<BlockCache> file_block_cache = GetBlockCache(file_info.fd());
    return T(file_info, namenode_client_, storage_cache_, file_block_cache);
  }

  shared_ptr<BlockCache> GetBlockCache(int fd);
  int AddBlock(int fd, long long offset, shared_ptr<BlockInfo> block);
  unique_ptr<CrailOutputstream> DirectoryOuput(FileInfo file_info,
                                               long long position);
  int WriteDirectoryRecord(FileInfo directory, string &fname, long long offset,
                           int valid);

  shared_ptr<NamenodeClient> namenode_client_;
  shared_ptr<StorageCache> storage_cache_;
  unordered_map<int, shared_ptr<BlockCache>> block_cache_;
};
} // namespace crail

#endif /* CRAIL_STORE_H */
