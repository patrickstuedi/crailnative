#include "block_cache.h"

#include <iostream>

using namespace std;

BlockCache::BlockCache(int fd) : fd_(fd) {}

BlockCache::~BlockCache() {}

int BlockCache::PutBlock(long long offset, shared_ptr<BlockInfo> block) {
  cache_.insert({offset, block});
  return 0;
}

shared_ptr<BlockInfo> BlockCache::GetBlock(long long offset) {
  shared_ptr<BlockInfo> block = nullptr;
  auto iter = cache_.find(offset);
  if (iter != cache_.end()) {
    block = iter->second;
  }
  return block;
}
