#include <iostream>

#include "storage/reflex/reflex_storage_client.h"

using namespace std;

ReflexStorageClient::ReflexStorageClient() {}

ReflexStorageClient::~ReflexStorageClient() {}

int ReflexStorageClient::WriteData(int key, long long address,
                                   shared_ptr<ByteBuffer> buf) {
  cout << "writing using reflex client " << endl;
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  shared_ptr<ReflexFuture> future = Put(lba, buf);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

int ReflexStorageClient::ReadData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  cout << "reading using reflex client " << endl;
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  shared_ptr<ReflexFuture> future = Get(lba, buf);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

long long ReflexStorageClient::linearBlockAddress(long long address,
                                                  int sectorSize) {
  return address / (long)sectorSize;
}
