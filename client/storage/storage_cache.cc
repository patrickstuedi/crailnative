#include "storage_cache.h"
#include <iostream>

#include "common/crail_constants.h"
#include "storage/narpc/narpc_storage_client.h"
#include "storage/reflex/reflex_storage_client.h"

using namespace crail;

StorageCache::StorageCache() {}

StorageCache::~StorageCache() {}

int StorageCache::Put(long long position, shared_ptr<StorageClient> client) {
  long long key = ComputeKey(position);
  cache_.insert({key, client});
  return 0;
}

shared_ptr<StorageClient> StorageCache::Get(long long position,
                                            int storage_class) {
  long long key = ComputeKey(position);
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

long long StorageCache::ComputeKey(long long position) {
  long long count = position / kBlockSize;
  return count * kBlockSize;
}
