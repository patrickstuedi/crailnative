#include "reflex_header.h"

ReflexHeader::ReflexHeader(short type, long long ticket, long long lba,
                           int count)
    : type_(type), ticket_(ticket), lba_(lba), count_(count),
      payload_(nullptr) {
  this->magic_ = sizeof(short) * 2 + sizeof(long long) * 2 + sizeof(count);
}

ReflexHeader::ReflexHeader(short type, long long ticket, long long lba,
                           int count, shared_ptr<ByteBuffer> payload)
    : ReflexHeader(type, ticket, lba, count) {
  this->payload_ = payload;
}

ReflexHeader::ReflexHeader() : payload_(nullptr) {}

ReflexHeader::~ReflexHeader() {}

int ReflexHeader::Write(ByteBuffer &buf) const {
  buf.set_order(ByteOrder::LittleEndian);
  buf.PutShort(magic_);
  buf.PutShort(type_);
  buf.PutLong(ticket_);
  buf.PutLong(lba_);
  buf.PutInt(count_);

  return 0;
}

int ReflexHeader::Update(ByteBuffer &buf) {
  buf.set_order(ByteOrder::LittleEndian);
  this->magic_ = buf.GetShort();
  this->type_ = buf.GetShort();
  this->ticket_ = buf.GetLong();
  this->lba_ = buf.GetLong();
  this->count_ = buf.GetInt();
  return 0;
}
