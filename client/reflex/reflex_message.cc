#include "reflex_message.h"

ReflexMessage::ReflexMessage(short type, long long ticket, long long lba,
                             int count)
    : type_(type), ticket_(ticket), lba_(lba), count_(count),
      payload_(nullptr) {
  this->magic_ = sizeof(short) * 2 + sizeof(long long) * +sizeof(count);
}

ReflexMessage::ReflexMessage(short type, long long ticket, long long lba,
                             int count, shared_ptr<ByteBuffer> payload)
    : ReflexMessage(type, ticket, lba, count) {
  this->payload_ = payload;
}

ReflexMessage::ReflexMessage() : payload_(nullptr) {}

ReflexMessage::~ReflexMessage() {}

int ReflexMessage::Write(ByteBuffer &buf) const {
  buf.set_order(ByteOrder::LittleEndian);
  buf.PutShort(magic_);
  buf.PutShort(type_);
  buf.PutLong(ticket_);
  buf.PutLong(lba_);
  buf.PutInt(count_);

  return 0;
}

int ReflexMessage::Update(ByteBuffer &buf) {
  buf.set_order(ByteOrder::LittleEndian);
  this->magic_ = buf.GetShort();
  this->type_ = buf.GetShort();
  this->ticket_ = buf.GetLong();
  this->lba_ = buf.GetLong();
  this->count_ = buf.GetInt();
  return 0;
}
