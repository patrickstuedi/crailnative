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
#include "ioctl_request.h"
#include "lookup_request.h"
#include "remove_request.h"
#include "setfile_request.h"

NamenodeClient::NamenodeClient() : RpcClient(true) { this->counter_ = 1; }

NamenodeClient::~NamenodeClient() {}

shared_ptr<CreateResponse> NamenodeClient::Create(Filename &name, int type,
                                                  int storage_class,
                                                  int location_class,
                                                  int enumerable) {
  Createrequest createReq(name, type, storage_class, location_class,
                          enumerable);
  shared_ptr<CreateResponse> getblockRes = make_shared<CreateResponse>();
  if (RpcClient::IssueRequest(createReq, getblockRes) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return getblockRes;
}

shared_ptr<LookupResponse> NamenodeClient::Lookup(Filename &name) {
  LookupRequest lookupReq(name);
  shared_ptr<LookupResponse> lookupRes = make_shared<LookupResponse>();
  if (RpcClient::IssueRequest(lookupReq, lookupRes) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return lookupRes;
}

shared_ptr<GetblockResponse> NamenodeClient::GetBlock(long long fd,
                                                      long long token,
                                                      long long position,
                                                      long long capacity) {
  GetblockRequest get_block_req(fd, token, position, capacity);
  shared_ptr<GetblockResponse> get_block_res = make_shared<GetblockResponse>();
  if (RpcClient::IssueRequest(get_block_req, get_block_res) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return get_block_res;
}

shared_ptr<VoidResponse> NamenodeClient::SetFile(shared_ptr<FileInfo> file_info,
                                                 bool close) {
  SetfileRequest set_file_req(file_info, close);
  shared_ptr<VoidResponse> set_file_res = make_shared<VoidResponse>();
  if (RpcClient::IssueRequest(set_file_req, set_file_res) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return set_file_res;
}

shared_ptr<RemoveResponse> NamenodeClient::Remove(Filename &name,
                                                  bool recursive) {
  RemoveRequest remove_req(name, recursive);
  shared_ptr<RemoveResponse> remove_res = make_shared<RemoveResponse>();
  if (RpcClient::IssueRequest(remove_req, remove_res) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return remove_res;
}

shared_ptr<IoctlResponse> NamenodeClient::Ioctl(unsigned char op,
                                                Filename &name) {
  IoctlRequest ioctl_request(op, name);
  shared_ptr<IoctlResponse> ioctl_response = make_shared<IoctlResponse>();
  if (RpcClient::IssueRequest(ioctl_request, ioctl_response) < 0) {
    return nullptr;
  }
  if (RpcClient::PollResponse() < 0) {
    return nullptr;
  }
  return ioctl_response;
}
