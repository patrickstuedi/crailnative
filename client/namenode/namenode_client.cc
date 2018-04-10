#include "namenode_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "crail_store.h"
#include "create_request.h"
#include "create_response.h"
#include "getblock_request.h"
#include "getblock_response.h"
#include "lookup_request.h"

NamenodeClient::NamenodeClient() { this->counter_ = 1; }

NamenodeClient::~NamenodeClient() {}

shared_ptr<CreateResponse> NamenodeClient::Create(Filename &name, int type,
                                                  int storage_class,
                                                  int location_class,
                                                  int enumerable) {
  Createrequest createReq(name, type, storage_class, location_class,
                          enumerable);
  shared_ptr<CreateResponse> getblockRes = make_shared<CreateResponse>();
  RpcClient::IssueRequest(createReq, getblockRes);
  while (RpcClient::PollResponse() < 0)
    ;
  return getblockRes;
}

shared_ptr<LookupResponse> NamenodeClient::Lookup(Filename &name) {
  LookupRequest lookupReq(name);
  shared_ptr<LookupResponse> lookupRes = make_shared<LookupResponse>();
  RpcClient::IssueRequest(lookupReq, lookupRes);
  while (RpcClient::PollResponse() < 0)
    ;
  return lookupRes;
}

shared_ptr<GetblockResponse> NamenodeClient::GetBlock(long long fd,
                                                      long long token,
                                                      long long position,
                                                      long long capacity) {
  GetblockRequest getblockReq(fd, token, position, capacity);
  shared_ptr<GetblockResponse> getblockRes = make_shared<GetblockResponse>();
  RpcClient::IssueRequest(getblockReq, getblockRes);
  while (RpcClient::PollResponse() < 0)
    ;
  return getblockRes;
}
