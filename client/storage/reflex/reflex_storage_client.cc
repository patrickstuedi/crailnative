#include <iostream>

#include "storage/reflex/reflex_storage_client.h"

using namespace std;

ReflexStorageClient::ReflexStorageClient() {}

ReflexStorageClient::~ReflexStorageClient() {}

int ReflexStorageClient::WriteData(int key, long long address,
                                   shared_ptr<ByteBuffer> buf) {
  return 0;
}

int ReflexStorageClient::ReadData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  return 0;
}
