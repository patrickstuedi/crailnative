#include "storage_cache.h"
#include "storage/narpc/narpc_storage_client.h"
#include <iostream>

StorageCache::StorageCache() {}

StorageCache::~StorageCache() {}

int StorageCache::Put(long long key, shared_ptr<StorageClient> client) {
  cache_[key] = client;
  return 0;
}

shared_ptr<StorageClient> StorageCache::Get(long long key, int storage_class) {
  shared_ptr<StorageClient> client = cache_[key];
  if (!client) {
    client = make_shared<NarpcStorageClient>();
    this->cache_.insert({key, client});
  }
  return client;
}
