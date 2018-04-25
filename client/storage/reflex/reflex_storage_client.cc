#include <iostream>

#include "reflex/reflex_message.h"
#include "storage/reflex/reflex_storage_client.h"

using namespace std;

ReflexStorageClient::ReflexStorageClient() { this->counter_ = 1; }

ReflexStorageClient::~ReflexStorageClient() {}

int ReflexStorageClient::WriteData(int key, long long address,
                                   shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  long long ticket = counter_++;
  int count = buf->remaining() / kReflexBlockSize;
  ReflexMessage reflex_request(kCmdPut, ticket, lba, count, buf);
  shared_ptr<ReflexMessage> reflex_response =
      make_shared<ReflexMessage>(-1, -1, -1, -1);
  IssueRequest(reflex_request, reflex_response);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

int ReflexStorageClient::ReadData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  long long lba = linearBlockAddress(address, kReflexBlockSize);
  long long ticket = counter_++;
  int count = buf->remaining() / kReflexBlockSize;
  ReflexMessage reflex_request(kCmdGet, ticket, lba, count);
  shared_ptr<ReflexMessage> reflex_response =
      make_shared<ReflexMessage>(-1, -1, -1, -1, buf);
  IssueRequest(reflex_request, reflex_response);
  while (ReflexClient::PollResponse() < 0)
    ;
  return 0;
}

long long ReflexStorageClient::linearBlockAddress(long long address,
                                                  int sectorSize) {
  return address / (long)sectorSize;
}
