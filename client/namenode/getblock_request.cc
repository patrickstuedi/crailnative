#include "getblock_request.h"

GetblockRequest::GetblockRequest(long long fd, long long token,
                                 long long position, long long capacity)
    : NamenodeRequest(static_cast<short>(RpcCommand::Getblock),
                      static_cast<short>(RequestType::Getblock)),
      fd_(fd), token_(token), position_(position), capacity_(capacity) {}

GetblockRequest::~GetblockRequest() {}

int GetblockRequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  buf.PutLong(fd_);
  buf.PutLong(token_);
  buf.PutLong(position_);
  buf.PutLong(capacity_);

  return Size();
}

int GetblockRequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  fd_ = buf.GetLong();
  token_ = buf.GetLong();
  position_ = buf.GetLong();
  capacity_ = buf.GetLong();

  return Size();
}
