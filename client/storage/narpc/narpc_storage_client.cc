#include <iostream>

#include "read_request.h"
#include "read_request.h"
#include "read_response.h"
#include "storage/narpc/narpc_storage_client.h"
#include "write_request.h"
#include "write_response.h"

using namespace std;

NarpcStorageClient::NarpcStorageClient() {}

NarpcStorageClient::~NarpcStorageClient() {}

int NarpcStorageClient::WriteData(int key, long long address,
                                  shared_ptr<ByteBuffer> buf) {
  cout << "writing using narpc client" << endl;
  WriteRequest write_request(key, address, buf->remaining(), buf);
  shared_ptr<WriteResponse> write_response = make_shared<WriteResponse>();
  IssueRequest(write_request, write_response);
  while (RpcClient::PollResponse() < 0)
    ;
  return 0;
}

int NarpcStorageClient::ReadData(int key, long long address,
                                 shared_ptr<ByteBuffer> buf) {
  cout << "reading using narpc client" << endl;
  ReadRequest read_request(key, address, buf->remaining());
  shared_ptr<ReadResponse> read_response = make_shared<ReadResponse>(buf);
  IssueRequest(read_request, read_response);
  while (RpcClient::PollResponse() < 0)
    ;

  return 0;
}
