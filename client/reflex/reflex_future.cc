#include "reflex_future.h"

ReflexFuture::ReflexFuture(long long ticket, shared_ptr<ByteBuffer> buffer)
    : ticket_(ticket) {
  this->buffer_ = buffer;
}

ReflexFuture::~ReflexFuture() {}
