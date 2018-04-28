#include <iostream>

#include "storage/reflex/reflex_storage_client.h"

using namespace std;

ReflexStorageClient::ReflexStorageClient() {}

ReflexStorageClient::~ReflexStorageClient() {}

int ReflexStorageClient::WriteData(int key, long long address,
                                   shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  shared_ptr<ReflexFuture> future = Put(lba, buf);
  if (future != nullptr) {
    while (ReflexClient::PollResponse() < 0)
      ;
    return 0;
  } else {
    return -1;
  }
}

int ReflexStorageClient::ReadData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  shared_ptr<ReflexFuture> future = Get(lba, buf);
  if (future != nullptr) {
    while (ReflexClient::PollResponse() < 0)
      ;
    return 0;
  } else {
    return -1;
  }
}

long long ReflexStorageClient::linearBlockAddress(long long address,
                                                  int sector_size) {
  if ((address % sector_size) == 0) {
    return address / (long)sector_size;
  } else {
    return -1;
  }
}
