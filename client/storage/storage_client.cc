#include <iostream>

#include "storage_client.h"
#include "write_request.h"
#include "write_response.h"

using namespace std;

StorageClient::StorageClient() {}

StorageClient::~StorageClient() {}

int StorageClient::WriteData(int key, long long address,
                             shared_ptr<ByteBuffer> buf) {
  WriteRequest write_request(key, address, buf->remaining(), buf);
  shared_ptr<WriteResponse> write_response = make_shared<WriteResponse>();
  issueRequest(write_request, write_response);
  while (RpcClient::pollResponse() < 0)
    ;
  return 0;
}
