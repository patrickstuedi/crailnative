#include "block_cache.h"

BlockCache::BlockCache(int fd) : fd_(fd) {}

BlockCache::~BlockCache() {}

int BlockCache::PutBlock(long long offset, shared_ptr<BlockInfo> block) {
  cache_[offset] = block;
  return 0;
}

shared_ptr<BlockInfo> BlockCache::GetBlock(long long offset) {
  shared_ptr<BlockInfo> block = cache_[offset];
  return block;
}
