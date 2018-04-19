#include <iostream>

#include "reflex/reflex_message.h"
#include "storage/reflex/reflex_storage_client.h"

using namespace std;

ReflexStorageClient::ReflexStorageClient() {}

ReflexStorageClient::~ReflexStorageClient() {}

int ReflexStorageClient::WriteData(int key, long long address,
                                   shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, 0, 512);
  ReflexMessage reflex_request(0, 1, 1, lba, 0);
  shared_ptr<ReflexMessage> reflex_response = make_shared<ReflexMessage>();
  IssueRequest(reflex_request, reflex_response);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

int ReflexStorageClient::ReadData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, 0, 512);
  ReflexMessage reflex_request(0, 1, 1, lba, 0);
  shared_ptr<ReflexMessage> reflex_response = make_shared<ReflexMessage>();
  IssueRequest(reflex_request, reflex_response);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

long long ReflexStorageClient::linearBlockAddress(long long address,
                                                  long long remoteOffset,
                                                  int sectorSize) {
  return (address + remoteOffset) / (long)sectorSize;
}
