#include <iostream>

#include "narpc_read_request.h"
#include "narpc_read_response.h"
#include "narpc_storage_request.h"
#include "narpc_storage_response.h"
#include "narpc_write_request.h"
#include "narpc_write_response.h"
#include "storage/narpc/narpc_storage_client.h"

using namespace std;

NarpcStorageClient::NarpcStorageClient()
    : RpcClient(NarpcStorageClient::kNodelay) {}

NarpcStorageClient::~NarpcStorageClient() {}

shared_ptr<StorageResponse>
NarpcStorageClient::WriteData(int key, long long address,
                              shared_ptr<ByteBuffer> buf) {
  NarpcWriteRequest write_request(key, address, buf->remaining(), buf);
  shared_ptr<NarpcWriteResponse> write_response =
      make_shared<NarpcWriteResponse>(this);
  if (IssueRequest(write_request, write_response) < 0) {
    return nullptr;
  }
  return write_response;
}

shared_ptr<StorageResponse>
NarpcStorageClient::ReadData(int key, long long address,
                             shared_ptr<ByteBuffer> buf) {
  NarpcReadRequest read_request(key, address, buf->remaining());
  shared_ptr<NarpcReadResponse> read_response =
      make_shared<NarpcReadResponse>(this, buf);
  if (IssueRequest(read_request, read_response) < 0) {
    return nullptr;
  }
  return read_response;
}
