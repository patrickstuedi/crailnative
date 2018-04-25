#include "storage_cache.h"
#include "storage/narpc/narpc_storage_client.h"
#include "storage/reflex/reflex_storage_client.h"
#include <iostream>

StorageCache::StorageCache() {}

StorageCache::~StorageCache() {}

int StorageCache::Put(long long key, shared_ptr<StorageClient> client) {
  cache_.insert({key, client});
  return 0;
}

shared_ptr<StorageClient> StorageCache::Get(long long key, int storage_class) {
  auto iter = cache_.find(key);
  if (iter != cache_.end()) {
    return iter->second;
  } else {
    shared_ptr<StorageClient> client = CreateClient(storage_class);
    Put(key, client);
    return client;
  }
}

shared_ptr<StorageClient> StorageCache::CreateClient(int storage_class) {
  if (storage_class == 0) {
    return make_shared<NarpcStorageClient>();
  } else {
    return make_shared<ReflexStorageClient>();
  }
}
